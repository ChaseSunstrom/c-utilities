#include "test_vector.h"

bool test_vector_new() {
  Vector(i32) v = Vector_new(NULL);
  if (v->u_size == 0 && v->u_capacity == 4 && v->p_data && v) {
    return true;
  }
  return false;
}

bool test_vector_push_back() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  if (v->u_size == 1 && v->u_capacity == 4) {
    return true;
  }

  return false;
}

bool test_vector_at() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  if ((i32)Vector_at(v, 0) == 1) {
    return true;
  }
  return false;
}

bool test_vector_front() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  if ((i32)Vector_front(v) == 1) {
    return true;
  }
  return false;
}

bool test_vector_back() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  if ((i32)Vector_back(v) == 1) {
    return true;
  }
  return false;
}

bool test_vector_pop_back() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  Vector_pop_back(v);
  if (v->u_size == 0) {
    return true;
  }
  return false;
}

bool test_vector_insert() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  i32 j = 2;
  Vector_insert(v, 0, (void *)j);
  if ((i32)Vector_at(v, 0) == 2) {
    return true;
  }
  return false;
}

bool test_vector_erase() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  Vector_erase(v, 0);
  if (v->u_size == 0) {
    return true;
  }
  return false;
}

bool test_vector_clear() {
  Vector(i32) v = Vector_new(NULL);
  i32 i = 1;
  Vector_push_back(v, (void *)i);
  Vector_clear(v);
  if (v->u_size == 0) {
    return true;
  }
  return false;
}
