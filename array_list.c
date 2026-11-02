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

bool add_element(int id, const char *emp_name, bool is_permanent);

int main() {
  printf("==== REGISTERING NEW EMPLOYEES ====\n");
  add_element(1837, "Sai Rohith", true);
  add_element(98392, "Abhishek Sharma", false);

  printf("\n==== RETRIEVING EMPLOYEE RECORDS ====\n\n");
  for (int i = 0; i <= curr_idx; i++) {
    employee *emp_rcd = (employee *)init_elem + i;
    printf("Employee ID: %d\n", emp_rcd->emp_id);
  }
  printf("Added elements successfully\n");
  return 0;
}

/*
 * Creates a new employee structure object in memory and appends to a
 * "list". This attempts to perform the allocation immediately next to 
 * the latest allocated structure, effectively manufacturing a contiguous
 * sequence without having to use a standard array.
 * 
 * PS: sadly, this is a poor way of handling it: buggy and inefficient to
 * say the least. For starters, `mmap` is allocating the object at a different
 * virtual address vs intended, as we aren't passing the `MAP_FIXED` flag 
 * (which is also potentially dangerous). Resultantly, the traversal
 * in the `main` function above fails as the second employee element
 * is not exactly consecutively allocated to the first employee
 * object. It's a failed experiment, but a successful learning :)
 */
bool add_element(int id, const char *emp_name, bool is_permanent) {
  employee *new_emp_alloc;

  if (curr_idx == -1 && init_elem == NULL) {
    printf("[DEBUG] No person element currently exists, initializing a new "
           "dynamic "
           "array sequence\n");

    init_elem = new_emp_alloc = malloc(sizeof(employee));
  } else {
    printf("[DEBUG] Array sequence already initialized. Adding a new person "
           "entry\n");

    new_emp_alloc = (employee *)mmap((void *)(init_elem + (curr_idx + 1)),
                                     sizeof(employee), PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (new_emp_alloc == MAP_FAILED) {
      perror("[DEBUG] Memory allocation for new employee failed\n");
      return false;
    }
  }
  new_emp_alloc->emp_id = id;
  new_emp_alloc->name = emp_name;
  new_emp_alloc->is_permanent = is_permanent;
  curr_idx++;
  printf("[DEBUG] Added an element at address %p with index %d\n",
         (void *)new_emp_alloc, curr_idx);
  return true;
}
