#include "alloc.h"

static void _set_allocator_from_type(_Alloc_T type, _Allocator_T *p_allocator) {
  switch (type) {
  case _STACK_ALLOCATOR:
    p_allocator->alloc = _stack_alloc;
    p_allocator->realloc = NULL;
    break;
  case _GENERAL_ALLOCATOR:
    p_allocator->alloc = _general_alloc;
    p_allocator->realloc = _general_realloc;
    break;
  case _ARENA_ALLOCATOR:
    p_allocator->alloc = _arena_alloc;
    p_allocator->realloc = _arena_realloc;
    break;
  case _PAGE_ALLOCATOR:
    p_allocator->alloc = _page_alloc;
    p_allocator->realloc = _page_realloc;
    break;
  default:
    break;
  }
}

static _Alloc_Func_T _get_allocator_alloc(_Alloc_T type) {
  switch (type) {
  case _STACK_ALLOCATOR:
    return _stack_alloc;
  case _GENERAL_ALLOCATOR:
    return _general_alloc;
  case _ARENA_ALLOCATOR:
    return _arena_alloc;
  case _PAGE_ALLOCATOR:
    return _page_alloc;
  default:
    return NULL;
  }
}

_Allocator_T *_Allocator_default(void) {
  return _Allocator_new_1(_GENERAL_ALLOCATOR);
}

_Allocator_T *_Allocator_new_1(_Alloc_T type) {
  _Alloc_Func_T alloc = _get_allocator_alloc(type);
  _Allocator_T *p_allocator = alloc(sizeof(_Allocator_T), 1);
  p_allocator->type = type;
  p_allocator->u_size = 0;
  p_allocator->s_capacity = -1;
  p_allocator->u_used = 0;
  p_allocator->p_blocks = NULL;
  _set_allocator_from_type(type, p_allocator);
  return p_allocator;
}

_Allocator_T *_Allocator_new_2(_Alloc_T type, ssize_t s_capacity) {
  _Allocator_T *p_allocator = malloc(sizeof(_Allocator_T));
  p_allocator->type = type;
  p_allocator->u_size = 0;
  p_allocator->s_capacity = s_capacity;
  p_allocator->u_used = 0;
  p_allocator->p_blocks = NULL;
  _set_allocator_from_type(type, p_allocator);
  return p_allocator;
}

_Allocator_T *_Allocator_new_4(_Alloc_T type, ssize_t s_capacity,
                               _Alloc_Func_T alloc, _Realloc_Func_T realloc) {
  _Allocator_T *p_allocator = alloc(sizeof(_Allocator_T), 1);
  p_allocator->type = type;
  p_allocator->u_size = 0;
  p_allocator->s_capacity = s_capacity;
  p_allocator->u_used = 0;
  p_allocator->alloc = alloc;
  p_allocator->realloc = realloc;
  p_allocator->p_blocks = NULL;
  return p_allocator;
}

void _Allocator_free(_Allocator_T *p_allocator, void *p_data) {
  if (!p_allocator || !p_data) {
    return;
  }
  _Allocated_Block_T *p_block = p_allocator->p_blocks;
  _Allocated_Block_T *p_prev = NULL;
  while (p_block) {
    if (p_block->p_data == p_data) {
      p_allocator->u_size -= p_block->u_size;
      if (p_prev) {
        p_prev->p_next = p_block->p_next;
      } else {
        p_allocator->p_blocks = p_block->p_next;
      }
      if (p_allocator->type == _PAGE_ALLOCATOR) {
        _page_free(p_block->p_data, p_block->u_size, 1);
      } else if (p_block->dealloc) {
        p_block->dealloc(p_block->p_data);
      }
      p_block->dealloc(p_block);
      return;
    }
    p_prev = p_block;
    p_block = p_block->p_next;
  }
}

void _Allocator_free_all(_Allocator_T *p_allocator) {
  if (!p_allocator || !p_allocator->p_blocks) {
    return;
  }

  _Allocated_Block_T *p_block = p_allocator->p_blocks;

  while (p_block) {
    _Allocated_Block_T *p_next = p_block->p_next;
    if (p_allocator->type == _PAGE_ALLOCATOR) {
      _page_free(p_block->p_data, p_block->u_size, 1);
    } else if (p_block->dealloc) {
      p_block->dealloc(p_block->p_data);
    }
    p_block->dealloc(p_block);
    p_block = p_next;
  }
}

void _Allocator_free_everything(_Allocator_T *p_allocator) {
  if (!p_allocator) {
    return;
  }

  _Allocator_free_all(p_allocator);
  free(p_allocator);
}

void *_Allocator_alloc_2(_Allocator_T *p_allocator, size_t u_size) {
  return _Allocator_alloc_3(p_allocator, u_size, 1);
}

void *_Allocator_alloc_3(_Allocator_T *p_allocator, size_t u_size,
                         size_t u_amount) {
  if (p_allocator->type == _STACK_ALLOCATOR) {
    return _Allocator_alloc_4(p_allocator, u_size, u_amount, NULL);
  }
  return _Allocator_alloc_4(p_allocator, u_size, u_amount, free);
}

void *_Allocator_alloc_4(_Allocator_T *p_allocator, size_t u_size,
                         size_t u_amount, _Dealloc_Func_T dealloc) {
  if (!p_allocator ||
      p_allocator->s_capacity != -1 &&
          p_allocator->u_used + u_size * u_amount > p_allocator->s_capacity) {
    return NULL;
  }

  void *p_data = p_allocator->alloc(u_size, u_amount);

  if (!p_data) {
    return NULL;
  }

  p_allocator->u_size += u_size * u_amount;
  p_allocator->u_used += u_size * u_amount;
  _Allocated_Block_T *p_block =
      p_allocator->alloc(sizeof(_Allocated_Block_T), 1);
  p_block->p_data = p_data;
  p_block->u_size = u_size * u_amount;
  p_block->dealloc = dealloc;
  p_block->p_next = p_allocator->p_blocks;
  p_allocator->p_blocks = p_block;
  return p_data;
}

void *_Allocator_realloc(_Allocator_T *p_allocator, void *p_data,
                         size_t u_size) {
  if (!p_allocator || !p_data) {
    return NULL;
  }
  if (p_allocator->type == _STACK_ALLOCATOR) {
    assert(false && "Cannot realloc with stack allocator.");
  }
  if (!p_allocator->realloc)
    assert(false &&
           "Cannot realloc with allocator that does not support realloc.");

  _Allocated_Block_T *p_block = p_allocator->p_blocks;
  while (p_block) {
    if (p_block->p_data == p_data) {
      size_t old_size = p_block->u_size;
      void *p_new_data = p_allocator->realloc(p_data, u_size, 1);
      if (!p_new_data) {
        return NULL;
      }
      p_block->p_data = p_new_data;
      p_block->u_size = u_size;
      p_allocator->u_size += u_size - old_size;
      return p_new_data;
    }
    p_block = p_block->p_next;
  }

  return NULL;
}
