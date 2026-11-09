#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef struct person {
  int emp_id;
  const char *name;
  bool is_permanent;
} employee;

int curr_idx = -1;
employee *init_elem;

bool reg_employee(int id, const char *emp_name, bool is_permanent);
void* __alloc_new_mem_page(size_t page_len_opt);

int main() {
  printf("==== REGISTERING NEW EMPLOYEES ====\n");
  reg_employee(1837, "Sai Rohith", true);
  reg_employee(1938, "Abhishek Sharma", false);

  printf("\n==== RETRIEVING EMPLOYEE RECORDS ====\n\n");
  for (int i = 0; i <= curr_idx; i++) {
    employee *emp_rcd = (employee *)init_elem + i * sizeof(employee);
    const char* emp_status = emp_rcd->is_permanent ? "PERMANENT" : "CONTRACTOR";
    printf("[%s] Employee ID: %d; Name: %s\n", emp_status, emp_rcd->emp_id, emp_rcd->name);
  }
  printf("Added elements successfully\n");
  return 0;
}

bool reg_employee(int id, const char *emp_name, bool is_permanent) {
  employee *new_emp_alloc;
  void* page;
  
  if (curr_idx == -1 && init_elem == NULL) {
    // This (almost) always means no paged mapping has been created yet,
    // so we ask one to be created with default page size.
    printf("Mapping a new memory segment\n");
    page = __alloc_new_mem_page(0);
    if (page == NULL) {
        printf("Error mapping memory, exiting");
	exit(EXIT_FAILURE);
    }

    // Initialize the first element at the base address of the mapped
    // address space.
    new_emp_alloc = (employee*)page;
    init_elem = new_emp_alloc;
  } else {
    new_emp_alloc = (employee*) (init_elem + ((curr_idx+1) * sizeof(employee)));
  }

  new_emp_alloc->emp_id = id;
  new_emp_alloc->name = emp_name;
  new_emp_alloc->is_permanent = is_permanent;
  curr_idx++;
  printf("[DEBUG] Added an element at address %p with index %d\n",
         (void *)new_emp_alloc, curr_idx);
  return true;
}

void* __alloc_new_mem_page(size_t page_len_opt) {
    // We silently fallback to 4096 if the user supplies a value not multiple of 4096, besides defaulting.
    size_t page_size_len = page_len_opt <= 0 || page_len_opt % 4096 != 0 ? 4096 : page_len_opt;
    void *page_map = mmap(NULL, page_size_len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    if (page_map == MAP_FAILED) {
        return NULL;
    }
    return page_map;
}
