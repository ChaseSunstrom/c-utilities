#include "unordered_map.h"

Unordered_Map *Unordered_Map_new_1(Allocator *p_allocator) {
  return Unordered_Map_new_2(murmur2_hash, p_allocator);
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
  if (!p_map) {
    return NULL;
  }

  p_map->hash_fn = hash_fn;
  p_map->key_destructor = key_destructor;
  p_map->value_destructor = value_destructor;
  p_map->p_container = Container_new(NULL, 0, 16, NULL, p_allocator);
  if (!p_map->p_container) {
    Allocator_free(p_allocator, p_map);
    return NULL;
  }

  p_map->p_container->destructor = Unordered_Map_Entry_destructor;
  p_map->p_container->b_external_allocator = b_external_allocator;

  return p_map;
}

void *Unordered_Map_at_2(Unordered_Map *p_map, const void *p_key) {
  // Assumes p_key is a null-terminated string
  return Unordered_Map_at_3(p_map, p_key, strlen(p_key));
}

void *Unordered_Map_at_3(Unordered_Map *p_map, const void *p_key,
                         size_t u_key_size) {
  if (!p_map || !p_key)
    return NULL;

  size_t u_index =
      p_map->hash_fn(p_key, u_key_size) % p_map->p_container->u_capacity;
  Unordered_Map_Entry *p_entry = p_map->p_container->p_data[u_index];
  while (p_entry) {
    if (p_entry->p_pair->u_key_size == u_key_size &&
        memcmp(p_entry->p_pair->p_key, p_key, u_key_size) == 0) {
      return p_entry->p_pair->p_value;
    }
    p_entry = p_entry->p_next;
  }
  return NULL;
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Unordered_Map_free_(Unordered_Map **pp_map) {
  if (!pp_map) {
    return;
  }
  Unordered_Map_free(*pp_map);
  *pp_map = NULL;
}
#endif

void Unordered_Map_free(Unordered_Map *p_map) {
  if (!p_map) {
    return;
  }

  bool b_external_allocator = p_map->p_container->b_external_allocator;
  Allocator *p_allocator = p_map->p_container->p_allocator;

  Container_free(p_map->p_container);
  Allocator_free(p_allocator, p_map);

  if (!b_external_allocator) {
    Allocator_free_everything(p_allocator);
  }
}

void Unordered_Map_rehash(Unordered_Map *p_map, size_t u_new_capacity) {
  if (!p_map || u_new_capacity <= p_map->p_container->u_capacity)
    return;

  Container *p_new_container = Container_new(NULL, 0, u_new_capacity, NULL,
                                             p_map->p_container->p_allocator);
  if (!p_new_container)
    return;

  p_new_container->destructor = p_map->p_container->destructor;
  p_new_container->b_external_allocator =
      p_map->p_container->b_external_allocator;

  for (size_t i = 0; i < p_map->p_container->u_capacity; i++) {
    Unordered_Map_Entry *p_entry = p_map->p_container->p_data[i];
    while (p_entry) {
      size_t u_new_index =
          p_map->hash_fn(p_entry->p_pair->p_key, p_entry->p_pair->u_key_size) %
          u_new_capacity;
      Unordered_Map_Entry *p_new_entry = Allocator_alloc(
          p_new_container->p_allocator, sizeof(Unordered_Map_Entry));
      if (!p_new_entry) {
        Container_free(p_new_container);
        return;
      }
      *p_new_entry = *p_entry;
      p_new_entry->p_next = p_new_container->p_data[u_new_index];
      p_new_container->p_data[u_new_index] = p_new_entry;
      p_entry = p_entry->p_next;
    }
  }

  Container_free(p_map->p_container);
  p_map->p_container = p_new_container;
}

void Unordered_Map_insert_3(Unordered_Map *p_map, const void *p_key,
                            void *p_value) {
  if (!p_map || !p_key)
    return;

  Unordered_Map_insert_4(p_map, p_key, p_value, strlen(p_key));
}

void Unordered_Map_insert_4(Unordered_Map *p_map, const void *p_key,
                            void *p_value, size_t u_key_size) {
  if (!p_map || !p_key)
    return;

  float load_factor =
      (float)(p_map->p_container->u_size + 1) / p_map->p_container->u_capacity;
  if (load_factor > 0.75) {
    Unordered_Map_rehash(p_map, p_map->p_container->u_capacity * 2);
  }

  size_t u_index =
      p_map->hash_fn(p_key, u_key_size) % p_map->p_container->u_capacity;
  Unordered_Map_Entry *p_entry = p_map->p_container->p_data[u_index];

  while (p_entry) {
    if (p_entry->p_pair->u_key_size == u_key_size &&
        memcmp(p_entry->p_pair->p_key, p_key, u_key_size) == 0) {
      if (p_map->value_destructor) {
        p_map->value_destructor(p_entry->p_pair->p_value);
      }
      p_entry->p_pair->p_value = p_value;
      return;
    }
    p_entry = p_entry->p_next;
  }

  Unordered_Map_Entry *p_new_entry = Allocator_alloc(
      p_map->p_container->p_allocator, sizeof(Unordered_Map_Entry));
  if (!p_new_entry)
    return;

  Unordered_Pair *p_pair =
      Allocator_alloc(p_map->p_container->p_allocator, sizeof(Unordered_Pair));
  if (!p_pair) {
    Allocator_free(p_map->p_container->p_allocator, p_new_entry);
    return;
  }

  p_pair->p_key = Allocator_alloc(p_map->p_container->p_allocator, u_key_size);
  if (!p_pair->p_key) {
    Allocator_free(p_map->p_container->p_allocator, p_pair);
    Allocator_free(p_map->p_container->p_allocator, p_new_entry);
    return;
  }

  memcpy(p_pair->p_key, p_key, u_key_size);
  p_pair->p_value = p_value;
  p_pair->u_key_size = u_key_size;

  p_new_entry->p_pair = p_pair;
  p_new_entry->key_destructor = p_map->key_destructor;
  p_new_entry->value_destructor = p_map->value_destructor;
  p_new_entry->p_allocator = p_map->p_container->p_allocator;
  p_new_entry->p_next = p_map->p_container->p_data[u_index];
  p_map->p_container->p_data[u_index] = p_new_entry;

  p_map->p_container->u_size++;
}

void Unordered_Map_erase(Unordered_Map *p_map, const void *p_key,
                         size_t u_key_size) {
  if (!p_map || !p_key)
    return;

  size_t u_index =
      p_map->hash_fn(p_key, u_key_size) % p_map->p_container->u_capacity;
  Unordered_Map_Entry *p_entry = p_map->p_container->p_data[u_index];
  Unordered_Map_Entry *p_prev = NULL;

  while (p_entry) {
    if (p_entry->p_pair->u_key_size == u_key_size &&
        memcmp(p_entry->p_pair->p_key, p_key, u_key_size) == 0) {
      if (p_prev) {
        p_prev->p_next = p_entry->p_next;
      } else {
        p_map->p_container->p_data[u_index] = p_entry->p_next;
      }
      Unordered_Map_Entry_free(p_entry);
      p_map->p_container->u_size--;
      return;
    }
    p_prev = p_entry;
    p_entry = p_entry->p_next;
  }
}

void Unordered_Map_clear(Unordered_Map *p_map) {
  if (!p_map)
    return;

  for (size_t i = 0; i < p_map->p_container->u_capacity; i++) {
    Unordered_Map_Entry *p_entry = p_map->p_container->p_data[i];
    while (p_entry) {
      Unordered_Map_Entry *p_next = p_entry->p_next;
      Unordered_Map_Entry_free(p_entry);
      p_entry = p_next;
    }
    p_map->p_container->p_data[i] = NULL;
  }
  p_map->p_container->u_size = 0;
}

void Unordered_Map_Entry_free(Unordered_Map_Entry *p_entry) {
  if (!p_entry)
    return;

  if (p_entry->key_destructor) {
    p_entry->key_destructor(p_entry->p_pair->p_key);
  }
  if (p_entry->value_destructor) {
    p_entry->value_destructor(p_entry->p_pair->p_value);
  }

  Allocator_free(p_entry->p_allocator, p_entry->p_pair->p_key);
  Allocator_free(p_entry->p_allocator, p_entry->p_pair);
  Allocator_free(p_entry->p_allocator, p_entry);
}

void Unordered_Map_Entry_destructor(void *p_entry) {
  Unordered_Map_Entry_free((Unordered_Map_Entry *)p_entry);
}
