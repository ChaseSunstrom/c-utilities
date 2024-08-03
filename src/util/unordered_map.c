#include "unordered_map.h"

#define INITIAL_CAPACITY 4
#define LOAD_FACTOR_THRESHOLD 0.75

static size_t next_power_of_two(size_t u_n) {
    return 1 << (size_t)ceil(log2(u_n));
}

static size_t get_index(Unordered_Map *p_map, const void *p_key, size_t u_key_size) {
    size_t u_hash = p_map->hash_fn(p_key, u_key_size);
    return u_hash & (p_map->u_capacity - 1);
}

static bool keys_equal(const void *p_key1, const void *p_key2, size_t u_key_size) {
    return memcmp(p_key1, p_key2, u_key_size) == 0;
}

static bool should_grow(Unordered_Map *p_map) {
    return (float)p_map->u_size / p_map->u_capacity > LOAD_FACTOR_THRESHOLD;
}

static bool Unordered_Map_grow(Unordered_Map *p_map) {
    size_t u_new_capacity = p_map->u_capacity * 2;
    Unordered_Map_Entry *p_new_entries = Allocator_alloc(p_map->p_allocator, sizeof(Unordered_Map_Entry) * u_new_capacity);
    if (!p_new_entries) return false;

    memset(p_new_entries, 0, sizeof(Unordered_Map_Entry) * u_new_capacity);

    for (size_t i = 0; i < p_map->u_capacity; i++) {
        if (p_map->p_entries[i].b_occupied) {
            size_t u_index = get_index(p_map, p_map->p_entries[i].p_key, p_map->p_entries[i].u_key_size);
            while (p_new_entries[u_index].b_occupied) {
                u_index = (u_index + 1) & (u_new_capacity - 1);
            }
            p_new_entries[u_index] = p_map->p_entries[i];
        }
    }

    Allocator_free(p_map->p_allocator, p_map->p_entries);
    p_map->p_entries = p_new_entries;
    p_map->u_capacity = u_new_capacity;
    return true;
}

Unordered_Map *Unordered_Map_new_4(Hash_Fn hash_fn, Dealloc_Fn key_destructor,
                                   Dealloc_Fn value_destructor,
                                   Allocator *p_allocator) {
    bool b_external_allocator = true;
    if (!p_allocator) {
        p_allocator = Allocator_default();
        b_external_allocator = false;
    }  

    Unordered_Map *p_map = Allocator_alloc(p_allocator, sizeof(Unordered_Map));
    if (!p_map) return NULL;

    p_map->u_capacity = INITIAL_CAPACITY;
    p_map->u_size = 0;
    p_map->hash_fn = hash_fn;
    p_map->key_destructor = key_destructor;
    p_map->value_destructor = value_destructor;
    p_map->p_allocator = p_allocator;
    p_map->b_external_allocator = b_external_allocator;

    p_map->p_entries = Allocator_alloc(p_allocator, sizeof(Unordered_Map_Entry) * INITIAL_CAPACITY);
    if (!p_map->p_entries) {
        Allocator_free(p_allocator, p_map);
        return NULL;
    }

    memset(p_map->p_entries, 0, sizeof(Unordered_Map_Entry) * INITIAL_CAPACITY);
    return p_map;
}

void *Unordered_Map_at_3(Unordered_Map *p_map, const void *p_key, size_t u_key_size) {
    if (!p_map || !p_key) return NULL;

    size_t u_index = get_index(p_map, p_key, u_key_size);
    while (p_map->p_entries[u_index].b_occupied) {
        if (keys_equal(p_map->p_entries[u_index].p_key, p_key, u_key_size)) {
            return p_map->p_entries[u_index].p_value;
        }
        u_index = (u_index + 1) & (p_map->u_capacity - 1);
    }
    return NULL;
}

void Unordered_Map_insert_4(Unordered_Map *p_map, const void *p_key, void *p_value, size_t u_key_size) {
    if (!p_map || !p_key) return;

    if (should_grow(p_map)) {
        if (!Unordered_Map_grow(p_map)) return;
    }

    size_t u_index = get_index(p_map, p_key, u_key_size);
    while (p_map->p_entries[u_index].b_occupied) {
        if (keys_equal(p_map->p_entries[u_index].p_key, p_key, u_key_size)) {
            // Update existing entry
            if (p_map->value_destructor) p_map->value_destructor(p_map->p_entries[u_index].p_value);
            p_map->p_entries[u_index].p_value = p_value;
            return;
        }
        u_index = (u_index + 1) & (p_map->u_capacity - 1);
    }

    // Insert new entry
    void *p_key_copy = Allocator_alloc(p_map->p_allocator, u_key_size);
    if (!p_key_copy) return;
    memcpy(p_key_copy, p_key, u_key_size);
    
    if (p_map->p_entries[u_index].p_key && p_map->key_destructor)
      p_map->key_destructor(p_map->p_entries[u_index].p_key);

    p_map->p_entries[u_index].p_key = p_key_copy;
    p_map->p_entries[u_index].p_value = p_value;
    p_map->p_entries[u_index].u_key_size = u_key_size;
    p_map->p_entries[u_index].b_occupied = true;
    p_map->u_size++;
}

void Unordered_Map_erase(Unordered_Map *p_map, const void *p_key, size_t u_key_size) {
    if (!p_map || !p_key) return;

    size_t u_index = get_index(p_map, p_key, u_key_size);
    while (p_map->p_entries[u_index].b_occupied) {
        if (keys_equal(p_map->p_entries[u_index].p_key, p_key, u_key_size)) {
            if (p_map->key_destructor) p_map->key_destructor(p_map->p_entries[u_index].p_key);
            if (p_map->value_destructor) p_map->value_destructor(p_map->p_entries[u_index].p_value);
            p_map->p_entries[u_index].b_occupied = false;
            p_map->u_size--;
            return;
        }
        u_index = (u_index + 1) & (p_map->u_capacity - 1);
    }
}

void Unordered_Map_clear(Unordered_Map *p_map) {
    if (!p_map) return;

    for (size_t i = 0; i < p_map->u_capacity; i++) {
        if (p_map->p_entries[i].b_occupied) {
            if (p_map->key_destructor) p_map->key_destructor(p_map->p_entries[i].p_key);
            if (p_map->value_destructor) p_map->value_destructor(p_map->p_entries[i].p_value);
            p_map->p_entries[i].b_occupied = false;
        }
    }
    p_map->u_size = 0;
}

void Unordered_Map_free(Unordered_Map *p_map) {
    if (!p_map) return;

    Unordered_Map_clear(p_map);
    Allocator_free(p_map->p_allocator, p_map->p_entries);
    Allocator_free(p_map->p_allocator, p_map);

    if (!p_map->b_external_allocator) {
        Allocator_free_everything(p_map->p_allocator);
    }
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Unordered_Map_free_(Unordered_Map **pp_map) {
    if (!pp_map || !*pp_map) return;
    Unordered_Map_free(*pp_map);
    *pp_map = NULL;
}
#endif

bool Unordered_Map_empty(Unordered_Map *p_map) {
    return p_map ? p_map->u_size == 0 : true;
}

bool Unordered_Map_rehash(Unordered_Map *p_map, size_t u_new_capacity) {
    if (!p_map || u_new_capacity <= p_map->u_capacity) return false;

    u_new_capacity = next_power_of_two(u_new_capacity);
    return Unordered_Map_grow(p_map);
}
