#ifndef CUTIL_LIST_H
#define CUTIL_LIST_H

#include "alloc.h"
#include "pch.h"
#include "system.h"

typedef struct List_Node {
  void *p_data;
  struct List_Node *p_next;
} List_Node;

typedef struct {
  List_Node *p_head;
  List_Node *p_tail;
  size_t u_size;
  bool b_external_allocator;
  void (*destructor)(void *);
  Allocator *p_allocator;
} List;

#define List(type) defer(List_free_) List *
#define List_new(...) CONCAT(List_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

List *List_new_1(Allocator *p_allocator);
List *List_new_2(Dealloc_Fn destructor, Allocator *p_allocator);
List *List_new_4(void *p_arr_data, size_t u_arr_size,
                 Dealloc_Fn destructor, Allocator *p_allocator);
void List_free(List *p_list);
void List_free_(List **p_list);
void List_push_back(List *p_list, void *p_data);
void List_pop_back(List *p_list);
void List_push_front(List *p_list, void *p_data);
void List_pop_front(List *p_list);
void List_remove(List *p_list, size_t u_index);
void List_insert(List *p_list, size_t u_index, void *p_data);
void List_erase(List *p_list, size_t u_index);
void List_clear(List *p_list);
void *List_at(List *p_list, size_t u_index);
void *List_front(List *p_list);
void *List_back(List *p_list);
bool List_empty(List *p_list);

#endif
