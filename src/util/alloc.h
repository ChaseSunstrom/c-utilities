#ifndef CUTIL_ALLOC_H
#define CUTIL_ALLOC_H

#include "mem.h"
#include "pch.h"

#define INITIAL_HASH_TABLE_SIZE 16
#define HASH_TABLE_LOAD_FACTOR 0.75

typedef enum
{
    GENERAL_ALLOCATOR,
    ARENA_ALLOCATOR,
    PAGE_ALLOCATOR,
} Alloc_T;

typedef struct Allocated_Block_T
{
    void* p_data;
    size_t u_size;
    Dealloc_Fn dealloc;
    size_t u_hash;
    struct Allocated_Block_T* next;  // For hash table chaining
} Allocated_Block;

typedef struct
{
    Allocated_Block** buckets;
    size_t capacity;
    size_t size;
} Block_Hash_Table;

typedef struct
{
    Alloc_T type;
    size_t u_size;
    ssize_t s_capacity;
    size_t u_used;
    Alloc_Fn alloc;
    Realloc_Fn realloc;
    size_t u_ref_count;
    Block_Hash_Table block_table;
} Allocator;

#define Allocator(type) Allocator *
#define Allocator_new(...) CONCAT(Allocator_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define Allocator_alloc(...) CONCAT(Allocator_alloc_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Allocator* Allocator_default(void);
Allocator* Allocator_new_1(Alloc_T type);
Allocator* Allocator_new_2(Alloc_T type, ssize_t s_capacity);
Allocator* Allocator_new_4(Alloc_T type, ssize_t s_capacity, Alloc_Fn alloc, Realloc_Fn realloc);
Allocator* Allocator_incref(Allocator* p_allocator);
Allocator* Allocator_decref(Allocator* p_allocator);
Allocator* Allocator_move(Allocator* p_allocator);
void Allocator_free(Allocator* p_allocator, void* p_data);
void Allocator_free_array(Allocator* p_allocator, void* p_data, size_t u_count, Dealloc_Fn dealloc);
void Allocator_free_with_dealloc(Allocator* p_allocator, void* p_data, Dealloc_Fn dealloc);
void Allocator_free_(Allocator** p_allocator);
void Allocator_free_all(Allocator* p_allocator);
void Allocator_free_everything(Allocator* p_allocator);
void* Allocator_alloc_2(Allocator* p_allocator, size_t u_size);
void* Allocator_alloc_3(Allocator* p_allocator, size_t u_size, size_t u_amount);
void* Allocator_alloc_4(Allocator* p_allocator, size_t u_size, size_t u_amount, Dealloc_Fn dealloc);
void* Allocator_realloc(Allocator* p_allocator, void* p_data, size_t u_size);

#endif // CUTIL_ALLOC_H