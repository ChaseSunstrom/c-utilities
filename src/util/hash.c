#include "hash.h"

// 1. DJB2 (Original version)
// Fast and simple, good for strings
size_t djb2_hash(const void *data, size_t size) {
  const u8 *p = (const u8 *)data;
  size_t hash = 5381;
  for (size_t i = 0; i < size; i++) {
    hash = ((hash << 5) + hash) + p[i];
  }
  return hash;
}

// 2. FNV-1a
// Simple and effective, good balance of speed and distribution
#define FNV_PRIME 0x01000193
#define FNV_OFFSET 0x811c9dc5
size_t fnv1a_hash(const void *data, size_t size) {
  const u8 *p = (const u8 *)data;
  size_t hash = FNV_OFFSET;
  for (size_t i = 0; i < size; i++) {
    hash ^= p[i];
    hash *= FNV_PRIME;
  }
  return hash;
}

// 3. Jenkins One-at-a-time
// Good distribution, a bit slower than FNV-1a
size_t jenkins_hash(const void *data, size_t size) {
  const u8 *p = (const u8 *)data;
  size_t hash = 0;
  for (size_t i = 0; i < size; i++) {
    hash += p[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash;
}

// 4. MurmurHash2
// Fast hash with good distribution
size_t murmur2_hash(const void *data, size_t size) {
  const size_t m = 0x5bd1e995;
  const size_t r = 24;
  size_t h = size ^ 0x5BD1E995;
  const size_t *p = (const size_t *)data;

  while (size >= 4) {
    size_t k = *p++;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    size -= 4;
  }

  const u8 *u_tail = (const u8 *)p;
  switch (size) {
  case 3:
    h ^= u_tail[2] << 16;
  case 2:
    h ^= u_tail[1] << 8;
  case 1:
    h ^= u_tail[0];
    h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

// 5. SipHash-2-4 (simplified, 64-bit version)
// Cryptographically strong, but slower
#define ROTL(x, b) (size_t)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND                                                               \
  do {                                                                         \
    v0 += v1;                                                                  \
    v1 = ROTL(v1, 13);                                                         \
    v1 ^= v0;                                                                  \
    v0 = ROTL(v0, 32);                                                         \
    v2 += v3;                                                                  \
    v3 = ROTL(v3, 16);                                                         \
    v3 ^= v2;                                                                  \
    v0 += v3;                                                                  \
    v3 = ROTL(v3, 21);                                                         \
    v3 ^= v0;                                                                  \
    v2 += v1;                                                                  \
    v1 = ROTL(v1, 17);                                                         \
    v1 ^= v2;                                                                  \
    v2 = ROTL(v2, 32);                                                         \
  } while (0)

size_t siphash(const void *data, size_t size) {
  const u8 *p = (const u8 *)data;
  size_t k0 = 0x0706050403020100ULL;
  size_t k1 = 0x0F0E0D0C0B0A0908ULL;
  size_t v0 = 0x736f6d6570736575ULL ^ k0;
  size_t v1 = 0x646f72616e646f6dULL ^ k1;
  size_t v2 = 0x6c7967656e657261ULL ^ k0;
  size_t v3 = 0x7465646279746573ULL ^ k1;
  size_t b = ((size_t)size) << 56;

  while (size >= 8) {
    size_t m = *(size_t *)p;
    v3 ^= m;
    for (size_t i = 0; i < 2; i++)
      SIPROUND;
    v0 ^= m;
    p += 8;
    size -= 8;
  }

  switch (size) {
  case 7:
    b |= ((size_t)p[6]) << 48;
  case 6:
    b |= ((size_t)p[5]) << 40;
  case 5:
    b |= ((size_t)p[4]) << 32;
  case 4:
    b |= ((size_t)p[3]) << 24;
  case 3:
    b |= ((size_t)p[2]) << 16;
  case 2:
    b |= ((size_t)p[1]) << 8;
  case 1:
    b |= ((size_t)p[0]);
  }

  v3 ^= b;
  for (size_t i = 0; i < 2; i++)
    SIPROUND;
  v0 ^= b;
  v2 ^= 0xff;
  for (size_t i = 0; i < 4; i++)
    SIPROUND;
  return v0 ^ v1 ^ v2 ^ v3;
}

// 6. XXHash (64-bit version)
#define PRIME32_1 2654435761U
#define PRIME32_2 2246822519U
#define PRIME32_3 3266489917U
#define PRIME32_4  668265263U
#define PRIME32_5  374761393U

size_t xxhash(const void* data, size_t length) {
    const uint8_t* p = (const uint8_t*)data;
    const uint8_t* end = p + length;
    size_t hash;

    if (length >= 16) {
        const uint8_t* limit = end - 16;
        size_t v1 = PRIME32_1 + PRIME32_2;
        size_t v2 = PRIME32_2;
        size_t v3 = 0;
        size_t v4 = -PRIME32_1;

        do {
            v1 += *((const uint32_t*)p) * PRIME32_2; p += 4;
            v1 = (v1 << 13) | (v1 >> 19); // Rotate left 13 bits
            v1 *= PRIME32_1;

            v2 += *((const uint32_t*)p) * PRIME32_2; p += 4;
            v2 = (v2 << 13) | (v2 >> 19); // Rotate left 13 bits
            v2 *= PRIME32_1;

            v3 += *((const uint32_t*)p) * PRIME32_2; p += 4;
            v3 = (v3 << 13) | (v3 >> 19); // Rotate left 13 bits
            v3 *= PRIME32_1;

            v4 += *((const uint32_t*)p) * PRIME32_2; p += 4;
            v4 = (v4 << 13) | (v4 >> 19); // Rotate left 13 bits
            v4 *= PRIME32_1;
        } while (p <= limit);

        hash = ((v1 << 1) | (v1 >> 31)) + ((v2 << 7) | (v2 >> 25)) + ((v3 << 12) | (v3 >> 20)) + ((v4 << 18) | (v4 >> 14));
    }
    else {
        hash = PRIME32_5;
    }

    hash += length;

    while (p + 4 <= end) {
        hash += *((const uint32_t*)p) * PRIME32_3;
        hash = ((hash << 17) | (hash >> 15)) * PRIME32_4;
        p += 4;
    }

    while (p < end) {
        hash += (*p) * PRIME32_5;
        hash = ((hash << 11) | (hash >> 21)) * PRIME32_1;
        p++;
    }

    hash ^= hash >> 15;
    hash *= PRIME32_2;
    hash ^= hash >> 13;
    hash *= PRIME32_3;
    hash ^= hash >> 16;

    return hash;
}