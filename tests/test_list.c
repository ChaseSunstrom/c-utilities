#include "test_list.h"

bool test_list_new() {
  List(NULL) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  return true;
}

bool test_list_push_back() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  return true;
}

bool test_list_pop_back() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  List_pop_back(p_list);
  return true;
}

bool test_list_push_front() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_front(p_list, &i);
  return p_list->u_size == 1;
}

bool test_list_pop_front() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_front(p_list, &i);
  List_pop_front(p_list);
  return p_list->u_size == 0;
}

bool test_list_remove() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  List_remove(p_list, 0);
  return p_list->u_size == 0;
}

bool test_list_insert() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_insert(p_list, 0, &i);
  return p_list->u_size == 1;
}

bool test_list_erase() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  List_erase(p_list, 0);
  return p_list->u_size == 0;
}

bool test_list_at() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  return *(i32 *)List_at(p_list, 0) == 1;
}

bool test_list_front() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  return *(i32 *)List_front(p_list) == 1;
}

bool test_list_back() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  i32 i = 1;
  List_push_back(p_list, &i);
  return *(i32 *)List_back(p_list) == 1;
}

bool test_list_empty() {
  List(i32) p_list = List_new(NULL);
  if (!p_list) {
    return false;
  }
  return List_empty(p_list);
}
