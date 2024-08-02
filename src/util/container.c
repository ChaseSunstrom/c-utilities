#include "container.h"

Container *Container_new(void *p_data, size_t u_data_size, size_t u_capacity,
                         void (*destructor)(void *), Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  if (!p_allocator) {
    return NULL;
  }

  Container *p_container = Allocator_alloc(p_allocator, sizeof(Container));
  if (!p_container) {
    return NULL;
  }

  p_container->u_capacity = u_capacity > 0 ? u_capacity : 4;
  p_container->u_size = p_data ? u_data_size : 0;
  p_container->destructor = destructor;
  p_container->p_allocator = p_allocator;
  p_container->b_external_allocator = b_external_allocator;
  p_container->p_data =
      Allocator_alloc(p_allocator, sizeof(void *) * p_container->u_capacity);

  if (!p_container->p_data) {
    Allocator_free(p_allocator, p_container);
    return NULL;
  }

  if (p_data) {
    memcpy(p_container->p_data, p_data, u_data_size * sizeof(void *));
  }
  memset(p_container->p_data + p_container->u_size, 0,
         (p_container->u_capacity - p_container->u_size) * sizeof(void *));

#if CUTIL_AUTO_CLEANUP_TYPES
  p_container->u_references = 1;
#endif

  return p_container;
}

void *Container_at(Container *p_container, size_t u_index) {
  if (!p_container || u_index >= p_container->u_size) {
    return NULL;
  }
  return p_container->p_data[u_index];
}

void Container_free(Container *p_container) {
  if (!p_container) {
    return;
  }
  Allocator *allocator = p_container->p_allocator;
  if (!allocator) {
    return;
  }
  if (p_container->destructor) {
    for (size_t i = 0; i < p_container->u_size; i++) {
      if (p_container->p_data[i]) {
        p_container->destructor(p_container->p_data[i]);
        p_container->p_data[i] = NULL;
      }
    }
  }
  Allocator_free(allocator, p_container->p_data);
  Allocator_free(allocator, p_container);
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Container_free_(Container **pp_container) {
  if (!pp_container || !*pp_container) {
    return;
  }

  Container *p_container = *pp_container;

  if (p_container->u_references > 1) {
    p_container->u_references--;
  } else {
    Container_free(p_container);
    *pp_container = NULL;
  }
}
#endif

void Container_insert(Container *p_container, size_t u_index, void *p_elem) {
  if (!p_container || u_index > p_container->u_size) {
    return;
  }

  // Check if we need to resize
  if (p_container->u_size >= p_container->u_capacity) {
    size_t new_capacity =
        p_container->u_capacity ? p_container->u_capacity * 2 : 4;
    void **new_data =
        Allocator_realloc(p_container->p_allocator, p_container->p_data,
                          sizeof(void *) * new_capacity);
    if (!new_data) {
      return; // Reallocation failed
    }
    p_container->p_data = new_data;
    p_container->u_capacity = new_capacity;
  }

  // Shift elements to make space for the new element
  for (size_t i = p_container->u_size; i > u_index; i--) {
    p_container->p_data[i] = p_container->p_data[i - 1];
  }

  // Insert the new element
  p_container->p_data[u_index] = p_elem;
  p_container->u_size++;
}

void Container_erase(Container *p_container, size_t u_index) {
  if (!p_container || u_index >= p_container->u_size) {
    return;
  }

  if (p_container->destructor) {
    p_container->destructor(p_container->p_data[u_index]);
  }

  for (size_t i = u_index; i < p_container->u_size - 1; i++) {
    p_container->p_data[i] = p_container->p_data[i + 1];
  }
  p_container->p_data[p_container->u_size - 1] = NULL;
  p_container->u_size--;
}

void Container_remove(Container *p_container, size_t u_index) {
  if (!p_container || u_index >= p_container->u_size) {
    return;
  }

  for (size_t i = u_index; i < p_container->u_size - 1; i++) {
    p_container->p_data[i] = p_container->p_data[i + 1];
  }

  p_container->u_size--;
}

void Container_clear(Container *p_container) {
  if (!p_container) {
    return;
  }

  if (p_container->destructor) {
    for (size_t i = 0; i < p_container->u_size; i++) {
      if (p_container->p_data[i]) {
        p_container->destructor(p_container->p_data[i]);
        p_container->p_data[i] = NULL;
      }
    }
  }

  p_container->u_size = 0;
}
