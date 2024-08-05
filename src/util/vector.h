#ifndef CUTIL_VECTOR_H
#define CUTIL_VECTOR_H

#include "alloc.h"
#include "container.h"
#include "mem.h"
#include "pch.h"

typedef struct {
  Container *container;
} Vector;

#define Vector(type) defer(Vector_free_) Vector *
#define Vector_new(...) CONCAT(Vector_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Vector *Vector_new_1(Allocator *p_allocator);
Vector *Vector_new_2(Dealloc_Fn destructor, Allocator *p_allocator);
Vector *Vector_new_3(size_t u_capacity, Dealloc_Fn destructor,
                     Allocator *p_allocator);
Vector *Vector_new_4(void *arr_data, size_t u_arr_size,
                     Dealloc_Fn destructor, Allocator *p_allocator);
void *Vector_at(Vector *p_vector, size_t u_index);
void *Vector_front(Vector *p_vector);
void *Vector_back(Vector *p_vector);
void Vector_free(Vector *p_vector);
void Vector_free_(Vector **pp_vector);
void Vector_push_back(Vector *p_vector, void *p_elem);
void Vector_pop_back(Vector *p_vector);
void Vector_insert(Vector *p_vector, size_t u_index, void *p_elem);
void Vector_erase(Vector *p_vector, size_t u_index);
void Vector_clear(Vector *p_vector);
bool Vector_empty(Vector *p_vector);

#endif
