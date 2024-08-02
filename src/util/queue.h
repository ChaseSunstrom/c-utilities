#ifndef CUTIL_QUEUE_H
#define CUTIL_QUEUE_H

#include "alloc.h"
#include "container.h"

typedef struct {
  Container *p_container;
} Queue;

#define Queue(type) CUTIL_AUTO_CLEANUP(Queue_free_) Queue *
#define Queue_new(...) CONCAT(Queue_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Queue *Queue_new_1(Allocator *p_allocator);
Queue *Queue_new_2(void (*destructor)(void *), Allocator *p_allocator);
void Queue_free(Queue *p_queue);
void Queue_free_(Queue **p_queue);
void Queue_enqueue(Queue *p_queue, void *p_data);
void *Queue_dequeue(Queue *p_queue);
void Queue_clear(Queue *p_queue);
bool Queue_empty(Queue *p_queue);

#endif
