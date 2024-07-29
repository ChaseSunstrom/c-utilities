#ifndef CUTIL_ALLOC_H
#define CUTIL_ALLOC_H

#include "mem.h"
#include "pch.h"

typedef enum {
  STACK_ALLOCATOR,
  GENERAL_ALLOCATOR,
  ARENA_ALLOCATOR,
  PAGE_ALLOCATOR,
} Alloc_T;

typedef void *(*Alloc_Fn)(size_t, size_t);
typedef void *(*Realloc_Fn)(void *, size_t, size_t);
typedef void (*Dealloc_Fn)(void *);

typedef struct Allocated_Block_T {
  void *p_data;
  size_t u_size;
  Dealloc_Fn dealloc;
} Allocated_Block;

typedef struct {
  Alloc_T type;
  size_t u_size;
  ssize_t s_capacity;
  size_t u_used;
  Alloc_Fn alloc;
  Realloc_Fn realloc;
  Allocated_Block *p_blocks;  // Pointer to dynamic array of Allocated_Block
  size_t blocks_capacity;     // Capacity of the dynamic array
  size_t blocks_size;         // Current size of the dynamic array
} Allocator;

#define Allocator(type) Allocator(type) *
#define Allocator_new(...) CONCAT(Allocator_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define Allocator_alloc(...) CONCAT(Allocator_alloc_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Allocator *Allocator_default(void);
Allocator *Allocator_new_1(Alloc_T type);
Allocator *Allocator_new_2(Alloc_T type, ssize_t s_capacity);
Allocator *Allocator_new_4(Alloc_T type, ssize_t s_capacity, Alloc_Fn alloc, Realloc_Fn realloc);
void Allocator_free(Allocator *p_allocator, void *p_data);
void Allocator_free_all(Allocator *p_allocator);
void Allocator_free_everything(Allocator *p_allocator);
void *Allocator_alloc_2(Allocator *p_allocator, size_t u_size);
void *Allocator_alloc_3(Allocator *p_allocator, size_t u_size, size_t u_amount);
void *Allocator_alloc_4(Allocator *p_allocator, size_t u_size, size_t u_amount, Dealloc_Fn dealloc);
void *Allocator_realloc(Allocator *p_allocator, void *p_data, size_t u_size);

#endif
