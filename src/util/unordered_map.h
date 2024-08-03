#ifndef CUTIL_UNORDERED_MAP_H
#define CUTIL_UNORDERED_MAP_H

#include "container.h"
#include "hash.h"

typedef struct {
  void *p_key;
  void *p_value;
  size_t u_key_size;
} Unordered_Pair;

typedef struct Unordered_Map_Entry {
  Unordered_Pair *p_pair;
  // Meta data related to the Unordered Map
  // Needed for easy deallocation
  Dealloc_Fn key_destructor;
  Dealloc_Fn value_destructor;
  Allocator *p_allocator;

  struct Unordered_Map_Entry *p_next;
} Unordered_Map_Entry;

typedef struct {
  Hash_Fn hash_fn;
  Dealloc_Fn key_destructor;
  Dealloc_Fn value_destructor;
  Container(Unordered_Map_Entry) p_container;
} Unordered_Map;

#define Unordered_Map(key, value) defer(Unordered_Map_free_) Unordered_Map *
#define Unordered_Map_new(...)                                                 \
  CONCAT(Unordered_Map_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define Unordered_Map_at(...)                                                  \
  CONCAT(Unordered_Map_at_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define Unordered_Map_insert(...)                                              \
  CONCAT(Unordered_Map_insert_, NARGS(__VA_ARGS__))(__VA_ARGS__)

void Unordered_Map_Entry_free(Unordered_Map_Entry *p_entry);
void Unordered_Map_Entry_destructor(void *p_entry);

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
void Unordered_Map_rehash(Unordered_Map *p_map, size_t u_capacity);
bool Unordered_Map_empty(Unordered_Map *p_map);

#endif
