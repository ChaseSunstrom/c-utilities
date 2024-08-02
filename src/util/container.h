#ifndef CUTIL_CONTAINER_H
#define CUTIL_CONTAINER_H

#include "alloc.h"
#include "pch.h"
#include "system.h"

typedef struct {
  size_t u_capacity;
  size_t u_size;
  void **p_data;
  void (*destructor)(void *);
  Allocator *p_allocator;
  bool b_external_allocator;
#if CUTIL_AUTO_CLEANUP_TYPES
  size_t u_references;
#endif
} Container;

#define Container(type) defer(Container_free_) Container *

Container *Container_new(void *p_data, size_t u_data_size, size_t u_capacity,
                         void (*destructor)(void *), Allocator *p_allocator);
void *Container_at(Container *p_container, size_t u_index);
void Container_free(Container *p_container);
void Container_free_(Container **pp_container);
void Container_insert(Container *p_container, size_t u_index, void *p_elem);
void Container_erase(Container *p_container, size_t u_index);
void Container_remove(Container *p_container, size_t u_index);
void Container_clear(Container *p_container);

#endif
