#include "mem.h"

#include "alloc.h"

void *stack_alloc(size_t u_size, size_t u_amount) {
  return alloca(u_size * u_amount);
}

void *general_alloc(size_t u_size, size_t u_amount) {
  return malloc(u_size * u_amount);
}

void *arena_alloc(size_t u_size, size_t u_amount) {
  return malloc(u_size * u_amount);
}

void *page_alloc(size_t u_size, size_t u_amount) {
  size_t total_size = u_size * u_amount;
#ifdef _WIN32
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  size_t page_size = si.dwPageSize;
#else
  size_t page_size = sysconf(_SC_PAGESIZE);
#endif

  // Round up to the nearest page size
  size_t alloc_size = (total_size + page_size - 1) & ~(page_size - 1);

#ifdef _WIN32
  void *p_ptr = VirtualAlloc(NULL, alloc_size + sizeof(size_t),
                             MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (p_ptr) {
    *(size_t *)p_ptr = alloc_size;
    p_ptr = (size_t *)p_ptr + 1;
  }
#else
  void *p_ptr = mmap(NULL, alloc_size + sizeof(size_t), PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (p_ptr == MAP_FAILED) {
    p_ptr = NULL;
  } else {
    *(size_t *)p_ptr = alloc_size;
    p_ptr = (size_t *)p_ptr + 1;
  }
#endif
  return p_ptr;
}

void page_free(void *p_ptr, size_t u_size, size_t u_amount) {
  if (!p_ptr) {
    return;
  }

  // Retrieve the allocation size stored at the beginning
  void *alloc_ptr = (size_t *)p_ptr - 1;
  size_t alloc_size = *(size_t *)alloc_ptr;

#ifdef _WIN32
  VirtualFree(alloc_ptr, 0, MEM_RELEASE);
#else
  munmap(alloc_ptr, alloc_size + sizeof(size_t));
#endif
}

void *general_realloc(void *p_ptr, size_t u_size, size_t u_amount) {
  return realloc(p_ptr, u_size * u_amount);
}

void *arena_realloc(void *p_ptr, size_t u_size, size_t u_amount) {
  return realloc(p_ptr, u_size * u_amount);
}

void *page_realloc(void *p_ptr, size_t u_size, size_t u_amount) {
  size_t new_size = u_size * u_amount;
  if (p_ptr == NULL) {
    return page_alloc(u_size, u_amount);
  }
  if (new_size == 0) {
    page_free(p_ptr, u_size, u_amount);
    return NULL;
  }

#ifdef _WIN32
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  size_t page_size = si.dwPageSize;
#else
  size_t page_size = sysconf(_SC_PAGESIZE);
#endif

  // Retrieve the old allocation size stored at the beginning
  void *alloc_ptr = (size_t *)p_ptr - 1;
  size_t old_alloc_size = *(size_t *)alloc_ptr;

  size_t new_alloc_size = (new_size + page_size - 1) & ~(page_size - 1);

  if (new_alloc_size <= old_alloc_size) {
    return p_ptr; // No need to reallocate
  }

  // Need to allocate a new, larger area
  void *new_p_ptr = page_alloc(u_size, u_amount);
  if (new_p_ptr == NULL) {
    return NULL; // Allocation failed
  }

  // Copy old data to new location
  memcpy(new_p_ptr, p_ptr, old_alloc_size);

  // Free the old allocation
  page_free(p_ptr, old_alloc_size, 1);

  return new_p_ptr;
}
