#ifndef CUTIL_VECTOR_H
#define CUTIL_VECTOR_H

#include "alloc.h"
#include "mem.h"
#include "pch.h"

typedef struct {
  size_t u_capacity;
  size_t u_size;
  void **p_data;
  void (*destructor)(void *);
  Allocator *p_allocator;
#if CUTIL_AUTO_CLEANUP_TYPES
  size_t u_references;
#endif
} Vector;

#define Vector(type) CUTIL_AUTO_CLEANUP(Vector_free_) Vector *
#define Vector_new(...) CONCAT(Vector_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Vector *Vector_new_1(void (*destructor)(void *));
Vector *Vector_new_2(void (*destructor)(void *), Allocator *p_allocator);
Vector *Vector_new_3(size_t u_capacity, void (*destructor)(void *),
                     Allocator *p_allocator);
Vector *Vector_new_4(void *arr_data, size_t u_arr_size,
                     void (*destructor)(void *), Allocator *p_allocator);
void *Vector_at(Vector *p_vector, size_t u_index);
void *Vector_front(Vector *p_vector);
void *Vector_back(Vector *p_vector);
void Vector_free(Vector *p_vector);
void Vector_free_(Vector **pp_vector);
void Vector_push_back(Vector *p_vector, void *p_elem);
void Vector_pop_back(Vector *p_vector);
void Vector_insert(Vector *p_vector, size_t u_index, void *p_elem);
void Vector_erase(Vector *p_vector, size_t u_index);
void Vector_clear(Vector *pp_vector);
bool Vector_empty(Vector *p_vector);

#endif
