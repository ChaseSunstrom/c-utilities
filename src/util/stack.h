#ifndef CUTIL_STACK_H
#define CUTIL_STACK_H

#include "alloc.h"
#include "container.h"

typedef struct {
  Container *p_container;
} Stack;

#define Stack(type) defer(Stack_free_) Stack *
#define Stack_new(...) CONCAT(Stack_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Stack *Stack_new_1(Allocator *p_allocator);
Stack *Stack_new_2(void (*destructor)(void *), Allocator *p_allocator);
void Stack_free(Stack *p_stack);
void Stack_free_(Stack **p_stack);
void Stack_push(Stack *p_stack, void *p_data);
void *Stack_pop(Stack *p_stack);
void *Stack_top(Stack *p_stack);
void Stack_clear(Stack *p_stack);
bool Stack_empty(Stack *p_stack);

#endif
