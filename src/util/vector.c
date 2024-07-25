#include "vector.h"

_Vector_T *_Vector_T_default(void) { return _Vector_T_new(8); }

_Vector_T *_Vector_T_new(u32 u_type_size) {
  return _Vector_T_new_cap(8, u_type_size);
}

_Vector_T *_Vector_T_new_cap(u64 u_capacity, u32 u_type_size) {
  _Vector_T *p_vector = malloc(sizeof(_Vector_T));
  p_vector->u_capacity = u_capacity;
  p_vector->u_size = 0;
  p_vector->u_type_size = u_type_size;

  i32 i_size = u_capacity * u_type_size;

  p_vector->p_data = malloc(i_size);
  memset(p_vector->p_data, 0, i_size);
  return p_vector;
}

_Vector_T *_Vector_T_new_arr(u64 u_capacity, u32 u_type_size, void *arr_data,
                             u64 u_arr_size) {
  _Vector_T *p_vector = malloc(sizeof(_Vector_T));
  p_vector->u_capacity = u_capacity;
  p_vector->u_size = u_arr_size;
  p_vector->u_type_size = u_type_size;
  p_vector->p_data = malloc(u_capacity * u_type_size);
  memcpy(p_vector->p_data, arr_data, u_arr_size * u_type_size);
  return p_vector;
}

void _Vector_T_free(_Vector_T *p_vector) {
  free(p_vector->p_data);
  free(p_vector);
}

void _Vector_T_push_back(_Vector_T *p_vector, void *p_elem) {
  if (p_vector->u_size == p_vector->u_capacity) {
    p_vector->u_capacity <<= 1;
    p_vector->p_data =
        realloc(p_vector->p_data, p_vector->u_capacity * p_vector->u_type_size);
  }

  // Copy the element into the vector
  // Makes sure to copy the element into the correct position because of the
  // type size
  p_vector->p_data[p_vector->u_size] = p_elem;
  p_vector->u_size++;
}

_Result_T _Vector_T_at(_Vector_T *p_vector, u64 u_index) {
  if (u_index >= p_vector->u_size) {
    return Result_new_err(
        void *, _Error_new(_ERROR_OUT_OF_BOUNDS,
                           _formatted_string(
                               "Index out of bounds when accessing vector "
                               "element: Index: %lu, "
                               "Size: %lu",
                               u_index, p_vector->u_size),
                           true));
  }

  return Result_new_ok(void *, p_vector->p_data[u_index]);
}
