#include "stack.h"

Stack *Stack_new_1(Allocator *p_allocator) {
  return Stack_new_2(NULL, p_allocator);
}

Stack *Stack_new_2(void (*destructor)(void *), Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  Stack *p_stack = Allocator_alloc(p_allocator, sizeof(Stack));
  if (!p_stack) {
    return NULL;
  }

  p_stack->p_container = Container_new(NULL, 0, 4, destructor, p_allocator);
  if (!p_stack->p_container) {
    Allocator_free(p_allocator, p_stack);
    return NULL;
  }
  p_stack->p_container->b_external_allocator = b_external_allocator;

  return p_stack;
}

void Stack_free(Stack *p_stack) {
  if (!p_stack) {
    return;
  }

  bool b_external_allocator = p_stack->p_container->b_external_allocator;

  Container_free(p_stack->p_container);
  Allocator_free(p_stack->p_container->p_allocator, p_stack);

  if (!b_external_allocator) {
    Allocator_free_everything(p_stack->p_container->p_allocator);
  }
}

void Stack_free_(Stack **p_stack) {
  if (!p_stack) {
    return;
  }
  Stack_free(*p_stack);
  *p_stack = NULL;
}

void Stack_push(Stack *p_stack, void *p_data) {
  if (!p_stack) {
    return;
  }

  Container_insert(p_stack->p_container, p_stack->p_container->u_size, p_data);
}

void *Stack_pop(Stack *p_stack) {
  if (!p_stack || !p_stack->p_container->u_size) {
    return NULL;
  }
  void *p_data =
      Container_at(p_stack->p_container, p_stack->p_container->u_size - 1);
  Container_remove(p_stack->p_container, p_stack->p_container->u_size - 1);
  return p_data;
}

void *Stack_top(Stack *p_stack) {
  if (!p_stack || !p_stack->p_container->u_size) {
    return NULL;
  }

  return Container_at(p_stack->p_container, p_stack->p_container->u_size - 1);
}

void Stack_clear(Stack *p_stack) {
  if (!p_stack) {
    return;
  }

  Container_clear(p_stack->p_container);
}

bool Stack_empty(Stack *p_stack) {
  return !p_stack || !p_stack->p_container->u_size;
}
