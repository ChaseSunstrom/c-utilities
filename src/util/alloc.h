#ifndef CUTIL_ALLOC_H
#define CUTIL_ALLOC_H

#ifndef CUTIL_ALLOC_INCLUDED
#define CUTIL_ALLOC_INCLUDED
#endif

#include "mem.h"
#include "pch.h"

typedef enum {
  _STACK_ALLOCATOR,
  _GENERAL_ALLOCATOR,
  _ARENA_ALLOCATOR,
  _PAGE_ALLOCATOR,
} _Alloc_T;

typedef void *(*_Alloc_Func_T)(size_t, size_t);
typedef void *(*_Realloc_Func_T)(void *, size_t, size_t);
typedef void (*_Dealloc_Func_T)(void *);
typedef struct _Allocated_Block_T {
  void *p_data;
  size_t u_size;
  _Dealloc_Func_T dealloc;
  struct _Allocated_Block_T *p_next;
} _Allocated_Block_T;

typedef struct {
  _Alloc_T type;
  size_t u_size;
  ssize_t s_capacity;
  size_t u_used;
  _Alloc_Func_T alloc;
  _Realloc_Func_T realloc;
  _Allocated_Block_T *p_blocks;
} _Allocator_T;

#define Allocator_T(type) _Allocator_T
#define Allocator(type) Allocator_T(type) *

#define Allocator_new(...)                                                     \
  CONCAT(_Allocator_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define Allocator_alloc(...)                                                   \
  CONCAT(_Allocator_alloc_, NARGS(__VA_ARGS__))(__VA_ARGS__)

_Allocator_T *_Allocator_default(void);
_Allocator_T *_Allocator_new_1(_Alloc_T type);
_Allocator_T *_Allocator_new_2(_Alloc_T type, ssize_t s_capacity);
_Allocator_T *_Allocator_new_4(_Alloc_T type, ssize_t s_capacity,
                               _Alloc_Func_T alloc, _Realloc_Func_T realloc);
void _Allocator_free(_Allocator_T *p_allocator, void *p_data);
void _Allocator_free_all(_Allocator_T *p_allocator);
void _Allocator_free_everything(_Allocator_T *p_allocator);
void *_Allocator_alloc_2(_Allocator_T *p_allocator, size_t u_size);
void *_Allocator_alloc_3(_Allocator_T *p_allocator, size_t u_size,
                         size_t u_amount);
void *_Allocator_alloc_4(_Allocator_T *p_allocator, size_t u_size,
                         size_t u_amount, _Dealloc_Func_T dealloc);
void *_Allocator_realloc(_Allocator_T *p_allocator, void *p_data,
                         size_t u_size);

#endif
