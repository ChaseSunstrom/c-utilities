#ifndef CUTIL_TYPES_H
#define CUTIL_TYPES_H

#include "system.h"
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

#if CUTIL_UNIX
#include <sys/types.h>
#else
typedef ptrdiff_t ssize_t;
#endif
#endif
