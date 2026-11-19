#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define DEFAULT_PAGE_SIZE 4096

typedef struct person {
  int emp_id;
  const char *name;
  bool is_permanent;
} employee;

int curr_idx = -1;
employee *init_elem;

bool reg_employee(int id, const char *emp_name, bool is_permanent);
void *__alloc_new_mem_page(size_t page_len_opt);
void __free_allocated_page(size_t page_len_opt);
bool __verify_page_len(size_t mem_page_len);

int main() {
  printf("==== REGISTERING NEW EMPLOYEES ====\n");
  reg_employee(1837, "Sai Rohith", true);
  reg_employee(1938, "Abhishek Sharma", false);

  printf("\n==== RETRIEVING EMPLOYEE RECORDS ====\n\n");
  for (int i = 0; i <= curr_idx; i++) {
    employee *emp_rcd = (employee *)(init_elem + i);
    const char *emp_status = emp_rcd->is_permanent ? "PERMANENT" : "CONTRACTOR";
    printf("[%s] Employee ID: %d; Name: %s\n", emp_status, emp_rcd->emp_id,
           emp_rcd->name);
  }
  printf("Added elements successfully\n");
  __free_allocated_page(0);
  return 0;
}

bool reg_employee(int id, const char *emp_name, bool is_permanent) {
  employee *new_emp_alloc;
  void *page;

  if (curr_idx == -1 && init_elem == NULL) {
    // This (almost) always means no paged mapping has been created yet,
    // so we ask one to be created with default page size.
    printf("[DEBUG] Mapping a new memory segment\n");
    page = __alloc_new_mem_page(0);
    if (page == NULL) {
      printf("Error mapping memory, exiting");
      exit(EXIT_FAILURE);
    }

    // Initialize the first element at the base address of the mapped
    // address space.
    new_emp_alloc = (employee *)page;
    init_elem = new_emp_alloc;
  } else {
    new_emp_alloc = (employee *)(init_elem + (curr_idx + 1));
  }

  new_emp_alloc->emp_id = id;
  new_emp_alloc->name = emp_name;
  new_emp_alloc->is_permanent = is_permanent;
  curr_idx++;
  printf("[DEBUG] Added an element at address %p with index %d\n",
         (void *)new_emp_alloc, curr_idx);
  return true;
}

void *__alloc_new_mem_page(size_t page_len_opt) {
  // We silently fallback to 4096 if the user supplies a value not multiple of
  // 4096, besides defaulting.
  size_t page_size_len =
      __verify_page_len(page_len_opt) ? page_len_opt : DEFAULT_PAGE_SIZE;
  void *page_map = mmap(NULL, page_size_len, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANON, -1, 0);

  if (page_map == MAP_FAILED) {
    return NULL;
  }
  return page_map;
}

void __free_allocated_page(size_t page_len_opt) {
  if (init_elem == NULL) {
    printf("[DEBUG] Initial page address is NULL");
    exit(EXIT_FAILURE);
  }
  if (munmap((void *)init_elem, __verify_page_len(page_len_opt)
                                    ? page_len_opt
                                    : DEFAULT_PAGE_SIZE) == -1) {
    printf("[DEBUG] There was an error clearing up the memory mapping");
  }
}

bool __verify_page_len(size_t mem_page_len) {
  return mem_page_len > 0 && mem_page_len % DEFAULT_PAGE_SIZE == 0;
}
