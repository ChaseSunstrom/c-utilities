#ifndef CUTIL_VECTOR_H
#define CUTIL_VECTOR_H

#include "mem.h"
#include "pch.h"
typedef struct {
  size_t u_capacity;
  size_t u_size;
  void **p_data;
  void (*destructor)(void *);
#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *p_allocator;
#endif
#if CUTIL_AUTO_CLEANUP_TYPES
  size_t u_references;
#endif
} _Vector_T;

#define Vector_T(type) _Vector_T
#define Vector(type) CUTIL_AUTO_CLEANUP(_Vector_T_free_) _Vector_T *
#define Vector_new(...) CONCAT(_Vector_T_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

_Vector_T *_Vector_T_new_1(void (*destructor)(void *));

#ifndef CUTIL_ALLOC_INCLUDED
_Vector_T *_Vector_T_new_2(size_t u_capacity, void (*destructor)(void *));
_Vector_T *_Vector_T_new_3(void *arr_data, size_t u_arr_size,
                           void (*destructor)(void *));
#else
_Vector_T *_Vector_T_new_2(void (*destructor)(void *),
                           _Allocator_T *p_allocator);
_Vector_T *_Vector_T_new_3(size_t u_capacity, void (*destructor)(void *),
                           _Allocator_T *p_allocator);
_Vector_T *_Vector_T_new_4(void *arr_data, size_t u_arr_size,
                           void (*destructor)(void *),
                           _Allocator_T *p_allocator);
#endif
void *_Vector_T_at(_Vector_T *p_vector, size_t u_index);
void *_Vector_T_front(_Vector_T *p_vector);
void *_Vector_T_back(_Vector_T *p_vector);
void _Vector_T_free(_Vector_T *p_vector);
void _Vector_T_free_(_Vector_T **pp_vector);
void _Vector_T_push_back(_Vector_T *p_vector, void *p_elem);
void _Vector_T_pop_back(_Vector_T *p_vector);
void _Vector_T_insert(_Vector_T *p_vector, size_t u_index, void *p_elem);
void _Vector_T_erase(_Vector_T *p_vector, size_t u_index);
void _Vector_T_clear(_Vector_T **pp_vector);
bool _Vector_T_empty(_Vector_T *p_vector);

#endif
