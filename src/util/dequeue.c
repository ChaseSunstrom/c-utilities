#include "dequeue.h"

Dequeue *Dequeue_new_1(Allocator *p_allocator) {
  return Dequeue_new_2(NULL, p_allocator);
}

Dequeue *Dequeue_new_2(void (*destructor)(void *), Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  Dequeue *p_dequeue = Allocator_alloc(p_allocator, sizeof(Dequeue));
  if (!p_dequeue) {
    return NULL;
  }

  p_dequeue->p_container = Container_new(NULL, 0, 4, destructor, p_allocator);
  if (!p_dequeue->p_container) {
    Allocator_free(p_allocator, p_dequeue);
    return NULL;
  }
  p_dequeue->p_container->b_external_allocator = b_external_allocator;

  return p_dequeue;
}

void Dequeue_free(Dequeue *p_dequeue) {
  if (!p_dequeue) {
    return;
  }

  bool b_external_allocator = p_dequeue->p_container->b_external_allocator;

  Container_free(p_dequeue->p_container);
  Allocator_free(p_dequeue->p_container->p_allocator, p_dequeue);

  if (!b_external_allocator) {
    Allocator_free_everything(p_dequeue->p_container->p_allocator);
  }
}

void Dequeue_free_(Dequeue **p_dequeue) {
  if (!p_dequeue) {
    return;
  }
  Dequeue_free(*p_dequeue);
  *p_dequeue = NULL;
}

void Dequeue_push_front(Dequeue *p_dequeue, void *p_data) {
  if (!p_dequeue) {
    return;
  }
  Container_insert(p_dequeue->p_container, 0, p_data);
}

void Dequeue_push_back(Dequeue *p_dequeue, void *p_data) {
  if (!p_dequeue) {
    return;
  }
  Container_insert(p_dequeue->p_container, p_dequeue->p_container->u_size,
                   p_data);
}

void *Dequeue_pop_front(Dequeue *p_dequeue) {
  if (!p_dequeue || !p_dequeue->p_container->u_size) {
    return NULL;
  }
  void *p_data = Container_at(p_dequeue->p_container, 0);
  Container_remove(p_dequeue->p_container, 0);
  return p_data;
}

void *Dequeue_pop_back(Dequeue *p_dequeue) {
  if (!p_dequeue || !p_dequeue->p_container->u_size) {
    return NULL;
  }
  void *p_data =
      Container_at(p_dequeue->p_container, p_dequeue->p_container->u_size - 1);
  Container_remove(p_dequeue->p_container, p_dequeue->p_container->u_size - 1);
  return p_data;
}

void *Dequeue_front(Dequeue *p_dequeue) {
  if (!p_dequeue || !p_dequeue->p_container->u_size) {
    return NULL;
  }
  return Container_at(p_dequeue->p_container, 0);
}

void *Dequeue_back(Dequeue *p_dequeue) {
  if (!p_dequeue || !p_dequeue->p_container->u_size) {
    return NULL;
  }
  return Container_at(p_dequeue->p_container,
                      p_dequeue->p_container->u_size - 1);
}

void Dequeue_clear(Dequeue *p_dequeue) {
  if (!p_dequeue) {
    return;
  }
  Container_clear(p_dequeue->p_container);
}

bool Dequeue_empty(Dequeue *p_dequeue) {
  return !p_dequeue || !p_dequeue->p_container->u_size;
}
