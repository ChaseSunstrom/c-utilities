#ifndef CUTIL_UNORDERED_MAP_H
#define CUTIL_UNORDERED_MAP_H

#include "alloc.h"
#include "hash.h"
#include "pch.h"

typedef struct {
    void *p_key;
    void *p_value;
    size_t u_key_size;
    bool b_occupied;
} Unordered_Map_Entry;

typedef struct Unordered_Map {
    Unordered_Map_Entry *p_entries;
    size_t u_capacity;
    size_t u_size;
    bool b_external_allocator;
    Hash_Fn hash_fn;
    Dealloc_Fn key_destructor;
    Dealloc_Fn value_destructor;
    Allocator *p_allocator;
} Unordered_Map;

#define Unordered_Map(key, value) defer(Unordered_Map_free_) Unordered_Map *

#define Unordered_Map_new(...) \
    CONCAT(Unordered_Map_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

#define Unordered_Map_at(...) \
    CONCAT(Unordered_Map_at_, NARGS(__VA_ARGS__))(__VA_ARGS__)

#define Unordered_Map_insert(...) \
    CONCAT(Unordered_Map_insert_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Unordered_Map *Unordered_Map_new_1(Allocator *p_allocator);
Unordered_Map *Unordered_Map_new_2(Hash_Fn hash_fn, Allocator *p_allocator);
Unordered_Map *Unordered_Map_new_4(Hash_Fn hash_fn, Dealloc_Fn key_destructor,
                                   Dealloc_Fn value_destructor,
                                   Allocator *p_allocator);

void *Unordered_Map_at_2(Unordered_Map *p_map, const void *p_key);
void *Unordered_Map_at_3(Unordered_Map *p_map, const void *p_key,
                         size_t u_key_size);

void Unordered_Map_insert_3(Unordered_Map *p_map, const void *p_key,
                            void *p_value);
void Unordered_Map_insert_4(Unordered_Map *p_map, const void *p_key,
                            void *p_value, size_t u_key_size);

void Unordered_Map_free(Unordered_Map *p_map);
void Unordered_Map_free_(Unordered_Map **pp_map);

void Unordered_Map_erase(Unordered_Map *p_map, const void *p_key,
                         size_t u_key_size);
void Unordered_Map_clear(Unordered_Map *p_map);
bool Unordered_Map_rehash(Unordered_Map *p_map, size_t u_new_capacity);
bool Unordered_Map_empty(Unordered_Map *p_map);

#endif // CUTIL_UNORDERED_MAP_H
