#include "list.h"

List *List_new_1(Allocator *p_allocator) {
  return List_new_2(NULL, p_allocator);
}

List *List_new_2(void (*destructor)(void *), Allocator *p_allocator) {

  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }

  List *p_list = Allocator_alloc(p_allocator, sizeof(List));
  if (!p_list) {
    return NULL;
  }

  p_list->p_head = NULL;
  p_list->p_tail = NULL;
  p_list->u_size = 0;
  p_list->b_external_allocator = b_external_allocator;
  p_list->destructor = destructor;
  p_list->p_allocator = p_allocator;

  return p_list;
}

List *List_new_4(void *p_arr_data, size_t u_arr_size,
                 void (*destructor)(void *), Allocator *p_allocator) {
  List *p_list = List_new_2(destructor, p_allocator);
  if (!p_list) {
    return NULL;
  }

  for (size_t i = 0; i < u_arr_size; i++) {
    List_push_back(p_list, p_arr_data + i);
  }

  return p_list;
}

#if CUTIL_AUTO_CLEANUP_TYPES
void List_free_(List **pp_list) {
  if (!pp_list) {
    return;
  }

  List_free(*pp_list);
  *pp_list = NULL;
}
#endif

void List_free(List *p_list) {
  if (!p_list) {
    return;
  }

  while (p_list->p_head) {
    List_Node *p_node = p_list->p_head;
    p_list->p_head = p_node->p_next;
    if (p_list->destructor) {
      p_list->destructor(p_node->p_data);
    }
  }

  if (!p_list->b_external_allocator) {
    Allocator_free_everything(p_list->p_allocator);
  }
}

void List_push_back(List *p_list, void *p_data) {
  List_Node *p_node = Allocator_alloc(p_list->p_allocator, sizeof(List_Node));
  if (!p_node) {
    return;
  }

  p_node->p_data = p_data;
  p_node->p_next = NULL;

  if (!p_list->p_head) {
    p_list->p_head = p_node;
    p_list->p_tail = p_node;
  } else {
    p_list->p_tail->p_next = p_node;
    p_list->p_tail = p_node;
  }

  p_list->u_size++;
}

void List_pop_back(List *p_list) {
  if (!p_list->p_head) {
    return;
  }

  List_Node *p_node = p_list->p_head;
  List_Node *p_prev = NULL;
  while (p_node->p_next) {
    p_prev = p_node;
    p_node = p_node->p_next;
  }

  if (p_prev) {
    p_prev->p_next = NULL;
    p_list->p_tail = p_prev;
  } else {
    p_list->p_head = NULL;
    p_list->p_tail = NULL;
  }

  if (p_list->destructor) {
    p_list->destructor(p_node->p_data);
  }
  Allocator_free(p_list->p_allocator, p_node);
  p_list->u_size--;
}

void List_push_front(List *p_list, void *p_data) {
  List_Node *p_node = Allocator_alloc(p_list->p_allocator, sizeof(List_Node));
  if (!p_node) {
    return;
  }

  p_node->p_data = p_data;
  p_node->p_next = p_list->p_head;

  if (!p_list->p_head) {
    p_list->p_tail = p_node;
  }

  p_list->p_head = p_node;
  p_list->u_size++;
}

void List_pop_front(List *p_list) {
  if (!p_list->p_head) {
    return;
  }

  List_Node *p_node = p_list->p_head;
  p_list->p_head = p_node->p_next;

  if (!p_list->p_head) {
    p_list->p_tail = NULL;
  }

  if (p_list->destructor) {
    p_list->destructor(p_node->p_data);
  }
  Allocator_free(p_list->p_allocator, p_node);
  p_list->u_size--;
}

void List_remove(List *p_list, size_t u_index) {
  if (u_index >= p_list->u_size) {
    return;
  }

  List_Node *p_node = p_list->p_head;
  List_Node *p_prev = NULL;
  for (size_t i = 0; i < u_index; i++) {
    p_prev = p_node;
    p_node = p_node->p_next;
  }

  if (p_prev) {
    p_prev->p_next = p_node->p_next;
  } else {
    p_list->p_head = p_node->p_next;
  }

  if (!p_node->p_next) {
    p_list->p_tail = p_prev;
  }

  if (p_list->destructor) {
    p_list->destructor(p_node->p_data);
  }
  Allocator_free(p_list->p_allocator, p_node);
  p_list->u_size--;
}

void List_insert(List *p_list, size_t u_index, void *p_data) {
  if (u_index >= p_list->u_size) {
    return;
  }

  List_Node *p_node = Allocator_alloc(p_list->p_allocator, sizeof(List_Node));
  if (!p_node) {
    return;
  }

  p_node->p_data = p_data;

  List_Node *p_prev = NULL;
  List_Node *p_curr = p_list->p_head;
  for (size_t i = 0; i < u_index; i++) {
    p_prev = p_curr;
    p_curr = p_curr->p_next;
  }

  p_node->p_next = p_curr;
  if (p_prev) {
    p_prev->p_next = p_node;
  } else {
    p_list->p_head = p_node;
  }

  p_list->u_size++;
}

void List_erase(List *p_list, size_t u_index) {
  if (u_index >= p_list->u_size) {
    return;
  }

  List_Node *p_node = p_list->p_head;
  List_Node *p_prev = NULL;
  for (size_t i = 0; i < u_index; i++) {
    p_prev = p_node;
    p_node = p_node->p_next;
  }

  if (p_prev) {
    p_prev->p_next = p_node->p_next;
  } else {
    p_list->p_head = p_node->p_next;
  }

  if (!p_node->p_next) {
    p_list->p_tail = p_prev;
  }

  if (p_list->destructor) {
    p_list->destructor(p_node->p_data);
  }
  Allocator_free(p_list->p_allocator, p_node);
  p_list->u_size--;
}

void List_clear(List *p_list) {
  while (p_list->p_head) {
    List_Node *p_node = p_list->p_head;
    p_list->p_head = p_node->p_next;
    if (p_list->destructor) {
      p_list->destructor(p_node->p_data);
    }
    Allocator_free(p_list->p_allocator, p_node);
  }

  p_list->p_tail = NULL;
  p_list->u_size = 0;
}

void *List_at(List *p_list, size_t u_index) {
  if (u_index >= p_list->u_size) {
    return NULL;
  }

  List_Node *p_node = p_list->p_head;
  for (size_t i = 0; i < u_index; i++) {
    p_node = p_node->p_next;
  }

  return p_node->p_data;
}

void *List_front(List *p_list) {
  if (!p_list->p_head) {
    return NULL;
  }

  return p_list->p_head->p_data;
}

void *List_back(List *p_list) {
  if (!p_list->p_tail) {
    return NULL;
  }

  return p_list->p_tail->p_data;
}

bool List_empty(List *p_list) { return p_list->u_size == 0; }
