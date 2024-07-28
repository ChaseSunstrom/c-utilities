#include "vector.h"

_Vector_T *_Vector_T_new_1(void (*destructor)(void *)) {

#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *allocator = _Allocator_default();
#else
  void *allocator = NULL;
#endif

  _Vector_T *p_vector = _auto_alloc(allocator, sizeof(_Vector_T), 1, NULL);

  if (!p_vector) {
    return NULL;
  }

  p_vector->u_capacity = 4;
  p_vector->u_size = 0;
  p_vector->destructor = destructor;
  p_vector->p_data =
      _auto_alloc(allocator, p_vector->u_capacity, sizeof(void *), destructor);

#ifdef CUTIL_ALLOC_INCLUDED
  p_vector->p_allocator = allocator;
#endif

#if CUTIL_AUTO_CLEANUP_TYPES
  p_vector->u_references = 1;
#endif

  if (!p_vector->p_data) {
    free(p_vector);
    return NULL;
  }

  memset(p_vector->p_data, 0, p_vector->u_capacity);
  return p_vector;
}

_Vector_T *_Vector_T_new_2(size_t u_capacity, void (*destructor)(void *)) {

#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *allocator = _Allocator_default();
#else
  void *allocator = NULL;
#endif

  _Vector_T *p_vector = _auto_alloc(allocator, sizeof(_Vector_T), 1, NULL);

  if (!p_vector) {
    return NULL;
  }

  p_vector->u_capacity = u_capacity > 0 ? u_capacity : 4;
  p_vector->u_size = 0;
  p_vector->destructor = destructor;
  p_vector->p_data =
      _auto_alloc(allocator, p_vector->u_capacity, sizeof(void *), destructor);

#if CUTIL_AUTO_CLEANUP_TYPES
  p_vector->u_references = 1;
#endif

  if (!p_vector->p_data) {
    free(p_vector);
    return NULL;
  }

  memset(p_vector->p_data, 0, u_capacity);
  return p_vector;
}

_Vector_T *_Vector_T_new_3(void *arr_data, size_t u_arr_size,
                           void (*destructor)(void *)) {
#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *allocator = _Allocator_default();
#else
  void *allocator = NULL;
#endif

  _Vector_T *p_vector =
      _auto_alloc(allocator, sizeof(_Vector_T), 1, destructor);

  if (!p_vector) {
    return NULL;
  }

  p_vector->u_capacity = u_arr_size * 2;
  p_vector->u_size = u_arr_size;
  p_vector->destructor = destructor;
  p_vector->p_data =
      _auto_alloc(allocator, p_vector->u_capacity, sizeof(void *), destructor);

  if (!p_vector->p_data) {
    free(p_vector);
    return NULL;
  }

  memcpy(p_vector->p_data, arr_data, u_arr_size * sizeof(void *));
  return p_vector;
}

#if CUTIL_AUTO_CLEANUP_TYPES
void _Vector_T_free_(_Vector_T **pp_vector) {
  _Vector_T *p_vector = *pp_vector;
  if (!p_vector) {
    return;
  }
  if (p_vector->u_references > 1) {
    p_vector->u_references--;
    return;
  }
  _Vector_T_free(*pp_vector);
}
#endif

void _Vector_T_free(_Vector_T *p_vector) {
#ifdef CUTIL_ALLOC_INCLUDED
  if (p_vector->p_allocator) {
    _Allocator_free_everything(p_vector->p_allocator);
  }
#endif
  if (!p_vector) {
    return;
  }

  if (!p_vector->p_data) {
    FREE_THEN_NULL(p_vector);
    return;
  }

  if (!p_vector->destructor) {
    FREE_THEN_NULL(p_vector->p_data);
    FREE_THEN_NULL(p_vector);
    return;
  }

  for (size_t i = 0; i < p_vector->u_size; i++) {
    p_vector->destructor(p_vector->p_data[i]);
  }

  FREE_THEN_NULL(p_vector->p_data);
  FREE_THEN_NULL(p_vector);
}

void _Vector_T_push_back(_Vector_T *p_vector, void *p_elem) {

#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *allocator = p_vector->p_allocator;
#else
  void *allocator = NULL;
#endif

  if (p_vector->u_size == p_vector->u_capacity) {
    p_vector->u_capacity *= 2;
    p_vector->p_data = _auto_realloc(allocator, p_vector->p_data,
                                     sizeof(void *), p_vector->u_capacity);
    if (!p_vector->p_data) {
      return;
    }
  }

  p_vector->p_data[p_vector->u_size] = p_elem;
  p_vector->u_size++;
}

void *_Vector_T_at(_Vector_T *p_vector, size_t u_index) {
  if (u_index >= p_vector->u_size) {
    return NULL;
  }
  return p_vector->p_data[u_index];
}

void *_Vector_T_front(_Vector_T *p_vector) {
  if (p_vector->u_size == 0) {
    return NULL;
  }
  return p_vector->p_data[0];
}

void *_Vector_T_back(_Vector_T *p_vector) {
  if (p_vector->u_size == 0) {
    return NULL;
  }
  return p_vector->p_data[p_vector->u_size - 1];
}

void _Vector_T_pop_back(_Vector_T *p_vector) {
  if (p_vector->u_size == 0) {
    return;
  }
  p_vector->u_size--;
}

void _Vector_T_insert(_Vector_T *p_vector, size_t u_index, void *p_elem) {
  if (u_index >= p_vector->u_size) {
    return;
  }

#ifdef CUTIL_ALLOC_INCLUDED
  _Allocator_T *allocator = p_vector->p_allocator;
#else
  void *allocator = NULL;
#endif

  if (p_vector->u_size == p_vector->u_capacity) {
    p_vector->u_capacity *= 2;
    p_vector->p_data = _auto_realloc(allocator, p_vector->p_data,
                                     sizeof(void *), p_vector->u_capacity);
  }

  // Shift all elements to the right
  for (size_t i = p_vector->u_size; i > u_index; i--) {
    p_vector->p_data[i] = p_vector->p_data[i - 1];
  }
  p_vector->p_data[u_index] = p_elem;
  p_vector->u_size++;
}

void _Vector_T_erase(_Vector_T *p_vector, size_t u_index) {
  if (u_index >= p_vector->u_size) {
    return;
  }

  // Shift all elements to the left
  for (size_t i = u_index; i < p_vector->u_size - 1; i++) {
    p_vector->p_data[i] = p_vector->p_data[i + 1];
  }

  p_vector->u_size--;
}

void _Vector_T_clear(_Vector_T **pp_vector) {
  if (!pp_vector) {
    return;
  }

  _Vector_T *p_vector = Vector_new(NULL);

  if (*pp_vector) {
    p_vector->destructor = (*pp_vector)->destructor;
  }

  _Vector_T_free(*pp_vector);
  *pp_vector = p_vector;
}

bool _Vector_T_empty(_Vector_T *p_vector) { return p_vector->u_size == 0; }
