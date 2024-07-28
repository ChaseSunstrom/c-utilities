#include "mem.h"

void *_auto_alloc(void *p_allocator, size_t u_size, size_t u_amount,
                  void (*dealloc)(void *)) {
#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *p_alloc = (_Allocator_T *)p_allocator;
  if (p_alloc) {
    return Allocator_alloc(p_alloc, u_size, u_amount, dealloc);
  }
#else
  return malloc(u_size * u_amount);
#endif
}

void _auto_free(void *p_allocator, void *p_data, size_t u_size,
                size_t u_amount) {

#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *p_alloc = (_Allocator_T *)p_allocator;
  if (p_alloc) {
    _Allocator_free(p_alloc, p_data);
    return;
  }
#else
  free(p_data);
#endif
}

void *_auto_realloc(void *p_allocator, void *p_data, size_t u_size,
                    size_t u_amount) {
#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *p_alloc = (_Allocator_T *)p_allocator;
  if (p_alloc) {
    return _Allocator_realloc(p_alloc, p_data, u_size * u_amount);
  }
#endif
  return realloc(p_data, u_size * u_amount);
}

void *_stack_alloc(size_t u_size, size_t u_amount) {
  return alloca(u_size * u_amount);
}

void *_general_alloc(size_t u_size, size_t u_amount) {
  return malloc(u_size * u_amount);
}

void *_arena_alloc(size_t u_size, size_t u_amount) {
  return malloc(u_size * u_amount);
}

void *_page_alloc(size_t u_size, size_t u_amount) {
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
  void *ptr =
      VirtualAlloc(NULL, alloc_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
  void *ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) {
    ptr = NULL;
  }
#endif
  return ptr;
}

void *_general_realloc(void *ptr, size_t u_size, size_t u_amount) {
  return realloc(ptr, u_size * u_amount);
}

void *_arena_realloc(void *ptr, size_t u_size, size_t u_amount) {
  return realloc(ptr, u_size * u_amount);
}

void *_page_realloc(void *ptr, size_t old_size, size_t new_size) {
  if (ptr == NULL) {
    return _page_alloc(new_size, 1);
  }

  if (new_size == 0) {
    _page_free(ptr, old_size, 1);
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
    return ptr;
  }

  // Need to allocate a new, larger area
  void *new_ptr = _page_alloc(new_size, 1);
  if (new_ptr == NULL) {
    return NULL; // Allocation failed
  }

  // Copy old data to new location
  memcpy(new_ptr, ptr, old_size);

  // Free the old allocation
  _page_free(ptr, old_size, 1);

  return new_ptr;
}

void _page_free(void *ptr, size_t u_size, size_t u_amount) {
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
  VirtualFree(ptr, 0, MEM_RELEASE);
#else
  munmap(ptr, alloc_size);
#endif
}
