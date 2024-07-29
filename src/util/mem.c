#include "mem.h"

#include "alloc.h"

void *auto_alloc(void *p_allocator, size_t u_size, size_t u_amount,
                 void (*dealloc)(void *)) {
  Allocator *p_alloc = (Allocator *)p_allocator;
  if (p_alloc) {
    return Allocator_alloc(p_alloc, u_size, u_amount, dealloc);
  }
}

void auto_free(void *p_allocator, void *p_data) {

  Allocator *p_alloc = (Allocator *)p_allocator;
  if (p_alloc) {
    Allocator_free(p_alloc, p_data);
    return;
  }
}

void *auto_realloc(void *p_allocator, void *p_data, size_t u_size,
                   size_t u_amount) {
  Allocator *p_alloc = (Allocator *)p_allocator;
  if (p_alloc) {
    return Allocator_realloc(p_alloc, p_data, u_size * u_amount);
  }
}

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
  void *p_ptr =
      VirtualAlloc(NULL, alloc_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
  void *p_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (p_ptr == MAP_FAILED) {
    p_ptr = NULL;
  }
#endif
  return p_ptr;
}

void *general_realloc(void *p_ptr, size_t u_size, size_t u_amount) {
  return realloc(p_ptr, u_size * u_amount);
}

void *arena_realloc(void *p_ptr, size_t u_size, size_t u_amount) {
  return realloc(p_ptr, u_size * u_amount);
}

void *page_realloc(void *p_ptr, size_t old_size, size_t new_size) {
  if (p_ptr == NULL) {
    return page_alloc(new_size, 1);
  }

  if (new_size == 0) {
    page_free(p_ptr, old_size, 1);
    return NULL;
  }

#ifdef _WIN32
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  size_t page_size = si.dwPageSize;
#else
  size_t page_size = sysconf(_SC_PAGESIZE);
#endif

  size_t old_alloc_size = (old_size + page_size - 1) & ~(page_size - 1);
  size_t new_alloc_size = (new_size + page_size - 1) & ~(page_size - 1);

  if (new_alloc_size <= old_alloc_size) {
    // Shrinking or same size, just return the original pointer
    return p_ptr;
  }

  // Need to allocate a new, larger area
  void *new_p_ptr = page_alloc(new_size, 1);
  if (new_p_ptr == NULL) {
    return NULL; // Allocation failed
  }

  // Copy old data to new location
  memcpy(new_p_ptr, p_ptr, old_size);

  // Free the old allocation
  page_free(p_ptr, old_size, 1);

  return new_p_ptr;
}

void page_free(void *p_ptr, size_t u_size, size_t u_amount) {
  size_t total_size = u_size * u_amount;

#ifdef _WIN32
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  size_t page_size = si.dwPageSize;
#else
  size_t page_size = sysconf(_SC_PAGESIZE);
#endif

  size_t alloc_size = (total_size + page_size - 1) & ~(page_size - 1);

#ifdef _WIN32
  VirtualFree(p_ptr, 0, MEM_RELEASE);
#else
  munmap(p_ptr, alloc_size);
#endif
}
