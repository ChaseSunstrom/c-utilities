#ifndef SPECTRA_VECTOR_H
#define SPECTRA_VECTOR_H

#include "pch.h"
#include "result.h"

typedef struct {
  u64 u_capacity;
  u64 u_size;
  u32 u_type_size;
  void **p_data;
} _Vector_T;

#define Vector_T(type) _Vector_T
#define Vector(type) Vector_T(type) *
#define Vector_new(...)                                                        \
  OVERLOAD_CHOOSER(__VA_ARGS__, _Vector_T_new_arr, _Vector_T_new_cap,          \
                   _Vector_T_new, _Vector_T_default)                           \
  (__VA_ARGS__)

_Vector_T *_Vector_T_default();
_Vector_T *_Vector_T_new(u32 u_type_size);
_Vector_T *_Vector_T_new_cap(u64 u_capacity, u32 u_type_size);
_Vector_T *_Vector_T_new_arr(u64 u_capacity, u32 u_type_size, void *arr_data,
                             u64 u_arr_size);
void _Vector_T_free(_Vector_T *p_vector);
void _Vector_T_push_back(_Vector_T *p_vector, void *p_elem);
void _Vector_T_pop_back(_Vector_T *p_vector);
void _Vector_T_insert(_Vector_T *p_vector, u64 u_index, void *p_elem);
void _Vector_T_erase(_Vector_T *p_vector, u64 u_index);
void _Vector_T_clear(_Vector_T *p_vector);
_Result_T _Vector_T_at(_Vector_T *p_vector, u64 u_index);
_Result_T _Vector_T_front(_Vector_T *p_vector);
_Result_T _Vector_T_back(_Vector_T *p_vector);
u64 _Vector_T_size(_Vector_T *p_vector);
u64 _Vector_T_capacity(_Vector_T *p_vector);
bool _Vector_T_empty(_Vector_T *p_vector);

#endif
