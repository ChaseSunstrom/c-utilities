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

void *_auto_alloc(void *p_allocator, size_t u_size, size_t u_amount,
                  void (*dealloc)(void *));
void *_auto_realloc(void *p_allocator, void *p_data, size_t u_size,
                    size_t u_amount);
void _auto_free(void *p_allocator, void *p_data, size_t u_size,
                size_t u_amount);

void *_stack_alloc(size_t u_size, size_t u_amount);
void *_general_alloc(size_t u_size, size_t u_amount);
void *_arena_alloc(size_t u_size, size_t u_amount);
void *_page_alloc(size_t u_size, size_t u_amount);

void *_general_realloc(void *p_data, size_t u_size, size_t u_amount);
void *_arena_realloc(void *p_data, size_t u_size, size_t u_amount);
void *_page_realloc(void *p_data, size_t u_size, size_t u_amount);

void _page_free(void *p_data, size_t u_size, size_t u_amount);

#endif
