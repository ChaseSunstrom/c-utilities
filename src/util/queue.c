#include "queue.h"

Queue *Queue_new_1(Allocator *p_allocator) {
  return Queue_new_2(NULL, p_allocator);
}

Queue *Queue_new_2(void (*destructor)(void *), Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  Queue *p_queue = Allocator_alloc(p_allocator, sizeof(Queue));
  if (!p_queue) {
    return NULL;
  }

  p_queue->p_container = Container_new(NULL, 0, 4, destructor, p_allocator);
  if (!p_queue->p_container) {
    Allocator_free(p_allocator, p_queue);
    return NULL;
  }
  p_queue->p_container->b_external_allocator = b_external_allocator;

  return p_queue;
}

void Queue_free(Queue *p_queue) {
  if (!p_queue) {
    return;
  }

  bool b_external_allocator = p_queue->p_container->b_external_allocator;

  Container_free(p_queue->p_container);
  Allocator_free(p_queue->p_container->p_allocator, p_queue);

  if (!b_external_allocator) {
    Allocator_free_everything(p_queue->p_container->p_allocator);
  }
}

void Queue_free_(Queue **p_queue) {
  if (!p_queue) {
    return;
  }
  Queue_free(*p_queue);
  *p_queue = NULL;
}

void Queue_enqueue(Queue *p_queue, void *p_data) {
  if (!p_queue) {
    return;
  }

  Container_insert(p_queue->p_container, p_queue->p_container->u_size, p_data);
}

void *Queue_dequeue(Queue *p_queue) {
  if (!p_queue || !p_queue->p_container->u_size) {
    return NULL;
  }
  void *p_data = Container_at(p_queue->p_container, 0);
  Container_remove(p_queue->p_container, 0);
  return p_data;
}

void Queue_clear(Queue *p_queue) {
  if (!p_queue) {
    return;
  }
  Container_clear(p_queue->p_container);
}

bool Queue_empty(Queue *p_queue) {
  return !p_queue || !p_queue->p_container->u_size;
}
