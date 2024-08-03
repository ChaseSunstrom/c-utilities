#ifndef CUITL_HASH_H
#define CUITL_HASH_H

#include "alloc.h"

typedef size_t (*Hash_Fn)(const void *p_data, size_t u_size);

size_t djb2_hash(const void *p_data, size_t u_size);
size_t fnv1a_hash(const void *p_data, size_t u_size);
size_t jenkins_hash(const void *p_data, size_t u_size);
size_t murmur2_hash(const void *p_data, size_t u_size);
size_t siphash(const void *p_data, size_t u_size);
size_t xxhash(const void *p_data, size_t u_size);

#endif
