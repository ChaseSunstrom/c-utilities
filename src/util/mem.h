#ifndef CUTIL_MEM_H
#define CUTIL_MEM_H

#include "pch.h"

#ifdef _MSC_VER
// MSVC
#include <malloc.h>
#define alloca _alloca
#elif defined(__GNUC__)
// GCC, Clang, Intel
#include <alloca.h>
#else
#ifndef NO_ALLOCA
// Fallback
#error                                                                         \
    "alloca() is not available on this platform, please define NO_ALLOCA to completely remove the feature."
#endif
#endif

void *stack_alloc(size_t u_size, size_t u_amount);
void *general_alloc(size_t u_size, size_t u_amount);
void *arena_alloc(size_t u_size, size_t u_amount);
void *page_alloc(size_t u_size, size_t u_amount);

void *general_realloc(void *p_data, size_t u_size, size_t u_amount);
void *arena_realloc(void *p_data, size_t u_size, size_t u_amount);
void *page_realloc(void *p_data, size_t u_size, size_t u_amount);

void page_free(void *p_data, size_t u_size, size_t u_amount);

#define SET_REF_COUNT(obj, value) ((obj)->u_ref_count = (value))

// Move macro
#define move(obj)                                                              \
  ({                                                                           \
    SET_REF_COUNT(obj, 1);                                                     \
    obj;                                                                       \
  })

#define copy(obj)                                                              \
  ({                                                                           \
    SET_REF_COUNT(obj, (obj)->u_ref_count + 1);                                \
    obj;                                                                       \
  })

#endif
