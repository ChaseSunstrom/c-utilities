#include "vector.h"

Vector *Vector_new_1(Allocator *p_allocator) {
  return Vector_new_3(4, NULL, p_allocator);
}

Vector *Vector_new_2(void (*destructor)(void *), Allocator *p_allocator) {
  return Vector_new_3(4, destructor, p_allocator);
}

Vector *Vector_new_3(size_t u_capacity, void (*destructor)(void *),
                     Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  Vector *p_vector = Allocator_alloc(p_allocator, sizeof(Vector));
  if (!p_vector) {
    return NULL;
  }

  p_vector->container =
      Container_new(NULL, 0, u_capacity, destructor, p_allocator);
  p_vector->container->b_external_allocator = b_external_allocator;
  if (!p_vector->container) {
    Allocator_free(p_allocator, p_vector);
    return NULL;
  }

  return p_vector;
}

Vector *Vector_new_4(void *arr_data, size_t u_arr_size,
                     void (*destructor)(void *), Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }

  Vector *p_vector = Allocator_alloc(p_allocator, sizeof(Vector));
  if (!p_vector) {
    return NULL;
  }

  p_vector->container = Container_new(arr_data, u_arr_size, u_arr_size * 2,
                                      destructor, p_allocator);
  p_vector->container->b_external_allocator = b_external_allocator;
  if (!p_vector->container) {
    Allocator_free(p_allocator, p_vector);
    return NULL;
  }

  return p_vector;
}

void Vector_free(Vector *p_vector) {
  if (!p_vector) {
    return;
  }
  Container *container = p_vector->container;
  if (!container) {
    return;
  }
  bool b_external_allocator = container->b_external_allocator;
  Allocator *allocator = container->p_allocator;
  if (!allocator) {
    return;
  }

  // Free the container (which will free its data)
  Container_free(container);

  // Free the vector
  Allocator_free(allocator, p_vector);

  // If it's a general allocator, we need to free everything
  if (allocator->type != STACK_ALLOCATOR && !b_external_allocator) {
    Allocator_free_everything(
        allocator); // Assuming allocator was created with malloc
  }
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Vector_free_(Vector **pp_vector) {
  if (!pp_vector || !*pp_vector) {
    return;
  }

  Vector_free(*pp_vector);
  *pp_vector = NULL;
}
#endif

void Vector_push_back(Vector *p_vector, void *p_elem) {
  if (!p_vector) {
    return;
  }

  Container_insert(p_vector->container, p_vector->container->u_size, p_elem);
}

void *Vector_at(Vector *p_vector, size_t u_index) {
  if (!p_vector) {
    return NULL;
  }

  return Container_at(p_vector->container, u_index);
}

void *Vector_front(Vector *p_vector) {
  if (!p_vector) {
    return NULL;
  }

  return Container_at(p_vector->container, 0);
}

void *Vector_back(Vector *p_vector) {
  if (!p_vector || p_vector->container->u_size == 0) {
    return NULL;
  }

  return Container_at(p_vector->container, p_vector->container->u_size - 1);
}

void Vector_pop_back(Vector *p_vector) {
  if (!p_vector || p_vector->container->u_size == 0) {
    return;
  }

  Container_remove(p_vector->container, p_vector->container->u_size - 1);
}

void Vector_insert(Vector *p_vector, size_t u_index, void *p_elem) {
  if (!p_vector) {
    return;
  }

  Container_insert(p_vector->container, u_index, p_elem);
}

void Vector_erase(Vector *p_vector, size_t u_index) {
  if (!p_vector || u_index >= p_vector->container->u_size) {
    return;
  }

  for (size_t i = u_index; i < p_vector->container->u_size - 1; i++) {
    p_vector->container->p_data[i] = p_vector->container->p_data[i + 1];
  }

  p_vector->container->u_size--;
}

void Vector_clear(Vector *p_vector) {
  if (!p_vector) {
    return;
  }

  if (p_vector->container->destructor) {
    for (size_t i = 0; i < p_vector->container->u_size; i++) {
      if (p_vector->container->p_data[i]) {
        p_vector->container->destructor(p_vector->container->p_data[i]);
        p_vector->container->p_data[i] = NULL;
      }
    }
  }

  p_vector->container->u_size = 0;
}

bool Vector_empty(Vector *p_vector) {
  return p_vector && p_vector->container->u_size == 0;
}
