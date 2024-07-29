#include "alloc.h"

static void _set_allocator_from_type(Alloc_T type, Allocator *p_allocator) {
  switch (type) {
  case STACK_ALLOCATOR:
    p_allocator->alloc = stack_alloc;
    p_allocator->realloc = NULL;
    break;
  case GENERAL_ALLOCATOR:
    p_allocator->alloc = general_alloc;
    p_allocator->realloc = general_realloc;
    break;
  case ARENA_ALLOCATOR:
    p_allocator->alloc = arena_alloc;
    p_allocator->realloc = arena_realloc;
    break;
  case PAGE_ALLOCATOR:
    p_allocator->alloc = page_alloc;
    p_allocator->realloc = page_realloc;
    break;
  default:
    break;
  }
}

static Alloc_Fn _get_allocator_alloc(Alloc_T type) {
  switch (type) {
  case STACK_ALLOCATOR:
    return stack_alloc;
  case GENERAL_ALLOCATOR:
    return general_alloc;
  case ARENA_ALLOCATOR:
    return arena_alloc;
  case PAGE_ALLOCATOR:
    return page_alloc;
  default:
    return NULL;
  }
}

static void _grow_blocks_array(Allocator *p_allocator) {
  if (!p_allocator) {
    return;
  }

  size_t new_capacity =
      p_allocator->blocks_capacity == 0 ? 4 : p_allocator->blocks_capacity * 2;
  Allocated_Block *new_blocks =
      p_allocator->alloc(sizeof(Allocated_Block) * new_capacity, 1);

  if (!new_blocks) {
    // Handle allocation failure
    return;
  }

  if (p_allocator->p_blocks) {
    // Copy existing blocks to the new array
    memcpy(new_blocks, p_allocator->p_blocks,
           p_allocator->blocks_size * sizeof(Allocated_Block));

    // Free the old blocks array
    if (p_allocator->type == PAGE_ALLOCATOR) {
      page_free(p_allocator->p_blocks,
                p_allocator->blocks_capacity * sizeof(Allocated_Block), 1);
    } else if (p_allocator->type == GENERAL_ALLOCATOR ||
               p_allocator->type == ARENA_ALLOCATOR) {
      free(p_allocator->p_blocks);
    }
  }

  p_allocator->p_blocks = new_blocks;
  p_allocator->blocks_capacity = new_capacity;
}

Allocator *Allocator_default(void) {
  return Allocator_new_1(GENERAL_ALLOCATOR);
}

Allocator *Allocator_new_1(Alloc_T type) {
  Alloc_Fn alloc = _get_allocator_alloc(type);
  Allocator *p_allocator = alloc(sizeof(Allocator), 1);
  if (!p_allocator) {
    return NULL;
  }
  p_allocator->type = type;
  p_allocator->u_size = 0;
  p_allocator->s_capacity = -1;
  p_allocator->u_used = 0;
  p_allocator->p_blocks = NULL;
  p_allocator->blocks_capacity = 0;
  p_allocator->blocks_size = 0;
  _set_allocator_from_type(type, p_allocator);
  return p_allocator;
}

Allocator *Allocator_new_2(Alloc_T type, ssize_t s_capacity) {
  Alloc_Fn alloc = _get_allocator_alloc(type);
  Allocator *p_allocator = alloc(sizeof(Allocator), 1);
  if (!p_allocator) {
    return NULL;
  }
  p_allocator->type = type;
  p_allocator->u_size = 0;
  p_allocator->s_capacity = s_capacity;
  p_allocator->u_used = 0;
  p_allocator->p_blocks = NULL;
  p_allocator->blocks_capacity = 0;
  p_allocator->blocks_size = 0;
  _set_allocator_from_type(type, p_allocator);
  return p_allocator;
}

Allocator *Allocator_new_4(Alloc_T type, ssize_t s_capacity, Alloc_Fn alloc,
                           Realloc_Fn realloc) {
  Allocator *p_allocator = alloc(sizeof(Allocator), 1);
  if (!p_allocator) {
    return NULL;
  }
  p_allocator->type = type;
  p_allocator->u_size = 0;
  p_allocator->s_capacity = s_capacity;
  p_allocator->u_used = 0;
  p_allocator->p_blocks = NULL;
  p_allocator->blocks_capacity = 0;
  p_allocator->blocks_size = 0;
  p_allocator->alloc = alloc;
  p_allocator->realloc = realloc;
  return p_allocator;
}

void Allocator_free(Allocator *p_allocator, void *p_data) {
  if (!p_allocator || !p_data) {
    return;
  }

  for (size_t i = 0; i < p_allocator->blocks_size; ++i) {
    if (p_allocator->p_blocks[i].p_data == p_data) {
      p_allocator->u_size -= p_allocator->p_blocks[i].u_size;

      if (p_allocator->type == PAGE_ALLOCATOR) {
        page_free(p_data, p_allocator->p_blocks[i].u_size, 1);
      } else if (p_allocator->p_blocks[i].dealloc) {
        p_allocator->p_blocks[i].dealloc(p_data);
      }

      // Remove the block by shifting the array
      memmove(&p_allocator->p_blocks[i], &p_allocator->p_blocks[i + 1],
              (p_allocator->blocks_size - i - 1) * sizeof(Allocated_Block));
      p_allocator->blocks_size--;
      return;
    }
  }
}

void Allocator_free_all(Allocator *p_allocator) {
  if (!p_allocator || !p_allocator->p_blocks) {
    return;
  }

  for (size_t i = 0; i < p_allocator->blocks_size; ++i) {
    if (p_allocator->type == PAGE_ALLOCATOR) {
      page_free(p_allocator->p_blocks[i].p_data,
                p_allocator->p_blocks[i].u_size, 1);
    } else if (p_allocator->p_blocks[i].dealloc) {
      p_allocator->p_blocks[i].dealloc(p_allocator->p_blocks[i].p_data);
    }
  }

  if (p_allocator->p_blocks) {
    if (p_allocator->type == PAGE_ALLOCATOR) {
      page_free(p_allocator->p_blocks,
                p_allocator->blocks_capacity * sizeof(Allocated_Block), 1);
    } else if (p_allocator->type == GENERAL_ALLOCATOR ||
               p_allocator->type == ARENA_ALLOCATOR) {
      free(p_allocator->p_blocks);
    }
  }

  p_allocator->p_blocks = NULL;
  p_allocator->blocks_capacity = 0;
  p_allocator->blocks_size = 0;
  p_allocator->u_size = 0;
  p_allocator->u_used = 0;
}

void Allocator_free_everything(Allocator *p_allocator) {
  if (!p_allocator) {
    return;
  }

  Allocator_free_all(p_allocator);

  // Free the allocator itself
  if (p_allocator->type == GENERAL_ALLOCATOR ||
      p_allocator->type == ARENA_ALLOCATOR) {
    free(p_allocator);
  } else if (p_allocator->type == PAGE_ALLOCATOR) {
    page_free(p_allocator, sizeof(Allocator), 1);
  }
  // Stack allocator does not need to be freed
}

void *Allocator_alloc_2(Allocator *p_allocator, size_t u_size) {
  return Allocator_alloc_3(p_allocator, u_size, 1);
}

void *Allocator_alloc_3(Allocator *p_allocator, size_t u_size,
                        size_t u_amount) {
  if (p_allocator->type == STACK_ALLOCATOR) {
    return Allocator_alloc_4(p_allocator, u_size, u_amount, NULL);
  }
  return Allocator_alloc_4(p_allocator, u_size, u_amount, free);
}

void *Allocator_alloc_4(Allocator *p_allocator, size_t u_size, size_t u_amount,
                        Dealloc_Fn dealloc) {
  if (!p_allocator ||
      (p_allocator->s_capacity != -1 &&
       p_allocator->u_used + u_size * u_amount > p_allocator->s_capacity)) {
    return NULL;
  }

  void *p_data = p_allocator->alloc(u_size * u_amount, 1);
  if (!p_data) {
    return NULL;
  }

  if (p_allocator->blocks_size == p_allocator->blocks_capacity) {
    _grow_blocks_array(p_allocator);
    if (p_allocator->blocks_size == p_allocator->blocks_capacity) {
      // If _grow_blocks_array failed, free the allocated memory and return NULL
      if (p_allocator->type == PAGE_ALLOCATOR) {
        page_free(p_data, u_size * u_amount, 1);
      } else if (dealloc) {
        dealloc(p_data);
      }
      return NULL;
    }
  }

  Allocated_Block new_block = {
      .p_data = p_data, .u_size = u_size * u_amount, .dealloc = dealloc};

  p_allocator->p_blocks[p_allocator->blocks_size++] = new_block;
  p_allocator->u_size += u_size * u_amount;
  p_allocator->u_used += u_size * u_amount;

  return p_data;
}

void *Allocator_realloc(Allocator *p_allocator, void *p_data, size_t u_size) {
  if (!p_allocator || !p_data) {
    return NULL;
  }
  if (p_allocator->type == STACK_ALLOCATOR) {
    assert(false && "Cannot realloc with stack allocator.");
    return NULL;
  }
  if (!p_allocator->realloc) {
    assert(false &&
           "Cannot realloc with allocator that does not support realloc.");
    return NULL;
  }

  for (size_t i = 0; i < p_allocator->blocks_size; ++i) {
    if (p_allocator->p_blocks[i].p_data == p_data) {
      size_t old_size = p_allocator->p_blocks[i].u_size;
      void *p_new_data = p_allocator->realloc(p_data, u_size, 1);
      if (!p_new_data) {
        return NULL;
      }
      p_allocator->p_blocks[i].p_data = p_new_data;
      p_allocator->p_blocks[i].u_size = u_size;
      p_allocator->u_size += u_size - old_size;
      p_allocator->u_used += u_size - old_size;
      return p_new_data;
    }
  }

  // If we get here, the data wasn't found in our blocks
  return NULL;
}
