#ifndef CUTILL_DEQUEUE_H
#define CUTILL_DEQUEUE_H

#include "alloc.h"
#include "container.h"

typedef struct {
  Container *p_container;
} Dequeue;

#define Dequeue(type) defer(Dequeue_free_) Dequeue *
#define Dequeue_new(...) CONCAT(Dequeue_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Dequeue *Dequeue_new_1(Allocator *p_allocator);
Dequeue *Dequeue_new_2(Dealloc_Fn destructor, Allocator *p_allocator);
void Dequeue_free(Dequeue *p_dequeue);
void Dequeue_free_(Dequeue **p_dequeue);
void *Dequeue_back(Dequeue *p_dequeue);
void Dequeue_push_back(Dequeue *p_dequeue, void *p_data);
void *Dequeue_pop_back(Dequeue *p_dequeue);
void *Dequeue_front(Dequeue *p_dequeue);
void Dequeue_push_front(Dequeue *p_dequeue, void *p_data);
void *Dequeue_pop_front(Dequeue *p_dequeue);
void Dequeue_clear(Dequeue *p_dequeue);
bool Dequeue_empty(Dequeue *p_dequeue);

#endif
