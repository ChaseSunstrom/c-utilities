#include "test_vector.h"
#include <assert.h>

bool test_vector_new() {
  Allocator* p_allocator = Allocator_new(PAGE_ALLOCATOR);
  Vector(i32) v = Vector_new(NULL);
  if (v && v->container && v->container->u_size == 0 && v->container->u_capacity == 4 && v->container->p_data) {
    return true;
  }
  return false;
}

bool test_vector_push_back() {
  Allocator* p_allocator = Allocator_new(GENERAL_ALLOCATOR);
  Vector(i32) v = Vector_new(NULL, p_allocator);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  bool result = (v->container->u_size == 1 && v->container->u_capacity == 4);
  Allocator_free_everything(p_allocator);
  return result;
}

bool test_vector_at() {
  Allocator *allocator = Allocator_new(PAGE_ALLOCATOR);
  Vector(i32) v = Vector_new_2(NULL, allocator);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  bool result = ((intptr_t)Vector_at(v, 0) == 1);
  return result;
}

bool test_vector_front() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  bool result = ((intptr_t)Vector_front(v) == 1);
  return result;
}

bool test_vector_back() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  bool result = ((intptr_t)Vector_back(v) == 1);
  return result;
}

bool test_vector_pop_back() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  Vector_pop_back(v);
  bool result = (v->container->u_size == 0);
  return result;
}

bool test_vector_insert() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  i32 j = 2;
  Vector_insert(v, 0, (void *)(intptr_t)j);
  bool result = ((intptr_t)Vector_at(v, 0) == 2);
  return result;
}

bool test_vector_erase() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  Vector_erase(v, 0);
  bool result = (v->container->u_size == 0);
  return result;
}

bool test_vector_clear() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)(intptr_t)i);
  Vector_clear(v);
  bool result = (v->container->u_size == 0);
  return result;
}
