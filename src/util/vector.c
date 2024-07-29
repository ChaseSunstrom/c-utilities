#include "vector.h"

Vector *Vector_new_1(void (*destructor)(void *)) {
    Allocator *p_allocator = Allocator_default();
    Vector *p_vector = Allocator_alloc(p_allocator, sizeof(Vector));

    if (!p_vector) {
        return NULL;
    }

    p_vector->u_capacity = 4;
    p_vector->u_size = 0;
    p_vector->destructor = destructor;
    p_vector->p_allocator = p_allocator;
    p_vector->p_data = Allocator_alloc(p_allocator, sizeof(void *) * p_vector->u_capacity);
    p_vector->u_references = 1;  // Initialize reference count

    if (!p_vector->p_data) {
        Allocator_free(p_allocator, p_vector);
        return NULL;
    }

    memset(p_vector->p_data, 0, p_vector->u_capacity * sizeof(void *));
    return p_vector;
}

Vector *Vector_new_2(void (*destructor)(void *), Allocator *p_allocator) {
    Vector *p_vector = Allocator_alloc(p_allocator, sizeof(Vector));

    if (!p_vector) {
        return NULL;
    }

    p_vector->u_capacity = 4;
    p_vector->u_size = 0;
    p_vector->destructor = destructor;
    p_vector->p_allocator = p_allocator;
    p_vector->p_data = Allocator_alloc(p_allocator, sizeof(void *) * p_vector->u_capacity);

    if (!p_vector->p_data) {
        Allocator_free(p_allocator, p_vector);
        return NULL;
    }

    memset(p_vector->p_data, 0, p_vector->u_capacity * sizeof(void *));
    return p_vector;
}

Vector *Vector_new_3(size_t u_capacity, void (*destructor)(void *), Allocator *p_allocator) {
    Vector *p_vector = Allocator_alloc(p_allocator, sizeof(Vector));

    if (!p_vector) {
        return NULL;
    }

    p_vector->u_capacity = u_capacity > 0 ? u_capacity : 4;
    p_vector->u_size = 0;
    p_vector->destructor = destructor;
    p_vector->p_allocator = p_allocator;
    p_vector->p_data = Allocator_alloc(p_allocator, sizeof(void *) * p_vector->u_capacity);

    if (!p_vector->p_data) {
        Allocator_free(p_allocator, p_vector);
        return NULL;
    }

    memset(p_vector->p_data, 0, p_vector->u_capacity * sizeof(void *));
    return p_vector;
}

Vector *Vector_new_4(void *arr_data, size_t u_arr_size, void (*destructor)(void *), Allocator *p_allocator) {
    Vector *p_vector = Allocator_alloc(p_allocator, sizeof(Vector));

    if (!p_vector) {
        return NULL;
    }

    p_vector->u_capacity = u_arr_size * 2;
    p_vector->u_size = u_arr_size;
    p_vector->destructor = destructor;
    p_vector->p_allocator = p_allocator;
    p_vector->p_data = Allocator_alloc(p_allocator, sizeof(void *) * p_vector->u_capacity);

    if (!p_vector->p_data) {
        Allocator_free(p_allocator, p_vector);
        return NULL;
    }

    memcpy(p_vector->p_data, arr_data, u_arr_size * sizeof(void *));
    memset(p_vector->p_data + u_arr_size, 0, (p_vector->u_capacity - u_arr_size) * sizeof(void *));
    return p_vector;
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Vector_free_(Vector **pp_vector) {
    if (!pp_vector || !*pp_vector) {
        return;
    }

    Vector *p_vector = *pp_vector;

    if (p_vector->u_references > 1) {
        p_vector->u_references--;
    } else {
        Vector_free(p_vector);
        *pp_vector = NULL;
    }
}
#endif

void Vector_free(Vector *p_vector) {
    if (!p_vector) {
        return;
    }

    Allocator *allocator = p_vector->p_allocator;
    if (!allocator) {
        return;
    }

    if (p_vector->destructor) {
        for (size_t i = 0; i < p_vector->u_size; i++) {
            if (p_vector->p_data[i]) {
                p_vector->destructor(p_vector->p_data[i]);
                p_vector->p_data[i] = NULL;
            }
        }
    }

    switch (allocator->type) {
        case GENERAL_ALLOCATOR:
            Allocator_free(allocator, p_vector->p_data);
            Allocator_free_everything(allocator);
            break;
        case ARENA_ALLOCATOR:
        case PAGE_ALLOCATOR:
            // For arena and page allocators, we only free the vector's data
            // The allocator and vector structure will be freed when the arena/page is freed
            Allocator_free(allocator, p_vector->p_data);
            Allocator_free(allocator, p_vector);
            break;
        case STACK_ALLOCATOR:
            // Stack allocator doesn't need explicit freeing
            // The memory will be automatically reclaimed when the stack frame is popped
            break;
        default:
            // Handle unknown allocator types
            assert(false && "Unknown allocator type");
            break;
    }
}

void Vector_push_back(Vector *p_vector, void *p_elem) {
    if (!p_vector) {
        return;
    }

    if (p_vector->u_size >= p_vector->u_capacity) {
        size_t new_capacity = p_vector->u_capacity * 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }

        void **new_data = Allocator_realloc(p_vector->p_allocator, p_vector->p_data, sizeof(void *) * new_capacity);
        if (!new_data) {
            return;
        }

        p_vector->p_data = new_data;
        p_vector->u_capacity = new_capacity;
    }

    p_vector->p_data[p_vector->u_size] = p_elem;
    p_vector->u_size++;
}

void *Vector_at(Vector *p_vector, size_t u_index) {
    if (u_index >= p_vector->u_size) {
        return NULL;
    }
    return p_vector->p_data[u_index];
}

void *Vector_front(Vector *p_vector) {
    if (p_vector->u_size == 0) {
        return NULL;
    }
    return p_vector->p_data[0];
}

void *Vector_back(Vector *p_vector) {
    if (p_vector->u_size == 0) {
        return NULL;
    }
    return p_vector->p_data[p_vector->u_size - 1];
}

void Vector_pop_back(Vector *p_vector) {
    if (p_vector->u_size == 0) {
        return;
    }
    p_vector->u_size--;
}

void Vector_insert(Vector *p_vector, size_t u_index, void *p_elem) {
    if (!p_vector || u_index > p_vector->u_size) {
        return;
    }

    if (p_vector->u_size == p_vector->u_capacity) {
        size_t new_capacity = p_vector->u_capacity * 2;
        void **new_data = Allocator_realloc(p_vector->p_allocator, p_vector->p_data, sizeof(void *) * new_capacity);
        if (!new_data) {
            return;
        }
        p_vector->p_data = new_data;
        p_vector->u_capacity = new_capacity;
    }

    for (size_t i = p_vector->u_size; i > u_index; i--) {
        p_vector->p_data[i] = p_vector->p_data[i - 1];
    }
    p_vector->p_data[u_index] = p_elem;
    p_vector->u_size++;
}

void Vector_erase(Vector *p_vector, size_t u_index) {
    if (u_index >= p_vector->u_size) {
        return;
    }

    for (size_t i = u_index; i < p_vector->u_size - 1; i++) {
        p_vector->p_data[i] = p_vector->p_data[i + 1];
    }

    p_vector->u_size--;
}

void Vector_clear(Vector *p_vector) {
    if (!p_vector) {
        return;
    }

    if (p_vector->destructor) {
        for (size_t i = 0; i < p_vector->u_size; i++) {
            if (p_vector->p_data[i]) {
                p_vector->destructor(p_vector->p_data[i]);
                p_vector->p_data[i] = NULL;  // Set to NULL after destruction
            }
        }
    }

    p_vector->u_size = 0;
}

bool Vector_empty(Vector *p_vector) {
    return p_vector->u_size == 0;
}
