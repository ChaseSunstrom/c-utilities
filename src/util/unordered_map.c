#include "unordered_map.h"

#define INITIAL_CAPACITY 4
#define LOAD_FACTOR_THRESHOLD 0.75

static inline size_t next_power_of_two(size_t u_n) {
    u_n--;
    u_n |= u_n >> 1;
    u_n |= u_n >> 2;
    u_n |= u_n >> 4;
    u_n |= u_n >> 8;
    u_n |= u_n >> 16;
    return u_n + 1;
}

static inline size_t get_index(size_t u_hash, size_t u_capacity) {
    return u_hash & (u_capacity - 1);
}

static bool keys_equal(const void *p_key1, const void *p_key2, size_t u_key_size) {
    return memcmp(p_key1, p_key2, u_key_size) == 0;
}

static bool should_grow(Unordered_Map *p_map) {
    return (float)p_map->u_size / p_map->u_capacity > LOAD_FACTOR_THRESHOLD;
}

static bool Unordered_Map_grow(Unordered_Map* p_map) {
    size_t u_new_capacity = p_map->u_capacity * 2;
    Unordered_Map_Entry* p_new_entries = Allocator_alloc(p_map->p_allocator, sizeof(Unordered_Map_Entry) * u_new_capacity);
    if (!p_new_entries) return false;

    memset(p_new_entries, 0, sizeof(Unordered_Map_Entry) * u_new_capacity);

    for (size_t i = 0; i < p_map->u_capacity; i++) {
        if (p_map->p_entries[i].b_occupied) {
            size_t u_index = get_index(p_map->p_entries[i].u_hash, u_new_capacity);
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

Unordered_Map *Unordered_Map_new_1(Allocator *p_allocator) {
	return Unordered_Map_new_4(NULL, NULL, NULL, p_allocator);
}

Unordered_Map *Unordered_Map_new_2(Hash_Fn hash_fn, Allocator *p_allocator) {
	return Unordered_Map_new_4(hash_fn, NULL, NULL, p_allocator);
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

void* Unordered_Map_at_3(Unordered_Map* p_map, const void* p_key, size_t u_key_size) {
    if (!p_map || !p_key) return NULL;

    size_t u_hash = p_map->hash_fn(p_key, u_key_size);
    size_t u_index = get_index(u_hash, p_map->u_capacity);
    size_t u_original_index = u_index;

    do {
        if (p_map->p_entries[u_index].b_occupied &&
            p_map->p_entries[u_index].u_hash == u_hash &&
            keys_equal(p_map->p_entries[u_index].p_key, p_key, u_key_size)) {
            return p_map->p_entries[u_index].p_value;
        }
        u_index = (u_index + 1) & (p_map->u_capacity - 1);
    } while (u_index != u_original_index && p_map->p_entries[u_index].b_occupied);

    return NULL;
}

void Unordered_Map_insert_4(Unordered_Map* p_map, const void* p_key, void* p_value, size_t u_key_size) {
    if (!p_map || !p_key) return;

    if (should_grow(p_map)) {
        if (!Unordered_Map_grow(p_map)) return;
    }

    size_t u_hash = p_map->hash_fn(p_key, u_key_size);
    size_t u_index = get_index(u_hash, p_map->u_capacity);
    size_t u_original_index = u_index;

    do {
        if (!p_map->p_entries[u_index].b_occupied) {
            // Insert new entry
            p_map->p_entries[u_index].p_key = p_key;
            p_map->p_entries[u_index].p_value = p_value;
            p_map->p_entries[u_index].u_key_size = u_key_size;
            p_map->p_entries[u_index].u_hash = u_hash;
            p_map->p_entries[u_index].b_occupied = true;
            p_map->u_size++;
            return;
        }

        if (p_map->p_entries[u_index].u_hash == u_hash &&
            keys_equal(p_map->p_entries[u_index].p_key, p_key, u_key_size)) {
            // Update existing entry
            if (p_map->value_destructor) p_map->value_destructor(p_map->p_entries[u_index].p_value);
            p_map->p_entries[u_index].p_value = p_value;
            return;
        }

        u_index = (u_index + 1) & (p_map->u_capacity - 1);
    } while (u_index != u_original_index);

    // If we get here, the map is full (shouldn't happen if we're resizing properly)
    assert(false && "Unordered Map is full");
}

void Unordered_Map_erase(Unordered_Map *p_map, const void *p_key, size_t u_key_size) {
    if (!p_map || !p_key) return;

    size_t u_index = get_index(p_map, p_key, u_key_size);
    while (p_map->p_entries[u_index].b_occupied) {
        if (keys_equal(p_map->p_entries[u_index].p_key, p_key, u_key_size)) {
            if (p_map->key_destructor) p_map->key_destructor(p_map->p_entries[u_index].p_key);
            if (p_map->value_destructor) p_map->value_destructor(p_map->p_entries[u_index].p_value);
            p_map->p_entries[u_index].p_key = NULL;
            p_map->p_entries[u_index].p_value = NULL;
            p_map->p_entries[u_index].b_occupied = false;
            p_map->u_size--;
            return;
        }
        u_index = (u_index + 1) & (p_map->u_capacity - 1);
    }
}

void Unordered_Map_clear(Unordered_Map* p_map) {
    if (!p_map) return;

    for (size_t i = 0; i < p_map->u_capacity; i++) {
        if (p_map->p_entries[i].b_occupied) {
            Allocator_free(p_map->p_allocator, p_map->p_entries[i].p_key);
            Allocator_free(p_map->p_allocator, p_map->p_entries[i].p_value);
            p_map->p_entries[i].b_occupied = false;
        }
    }

    p_map->u_size = 0;

    if (p_map->u_capacity > INITIAL_CAPACITY) {
        Unordered_Map_Entry* new_entries = Allocator_alloc(p_map->p_allocator, sizeof(Unordered_Map_Entry) * INITIAL_CAPACITY);
        if (new_entries) {
            Allocator_free(p_map->p_allocator, p_map->p_entries);
            p_map->p_entries = new_entries;
            p_map->u_capacity = INITIAL_CAPACITY;
            memset(p_map->p_entries, 0, sizeof(Unordered_Map_Entry) * INITIAL_CAPACITY);
        }
    }
}

void Unordered_Map_free(Unordered_Map *p_map) {
    if (!p_map) return;

    bool b_external_allocator = p_map->b_external_allocator;
    Allocator* p_allocator = p_map->p_allocator;

    Unordered_Map_clear(p_map);
    Allocator_free(p_allocator, p_map);

    if (!b_external_allocator) {
        Allocator_free_everything(p_allocator);
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
