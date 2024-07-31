#ifndef CUTIL_SYSTEM_H
#define CUTIL_SYSTEM_H

#define CUTIL_VERSION_MAJOR 0
#define CUTIL_VERSION_MINOR 1
#define CUTIL_VERSION_PATCH 0

#ifdef __unix
#define CUTIL_UNIX 1
#else
#define CUTIL_UNIX 0
#endif

#ifdef __linux
#define CUTIL_LINUX 1
#else
#define CUTIL_LINUX 0
#endif

#ifdef _WIN32
#define CUTIL_WIN32 1
#else
#define CUTIL_WIN32 0
#endif

#ifdef __APPLE__
#define CUTIL_APPLE 1
#else
#define CUTIL_APPLE 0
#endif

#if CUTIL_UNIX || CUTIL_LINUX || CUTIL_APPLE
#define CUTIL_POSIX 1
#else
#define CUTIL_POSIX 0
#endif

#ifndef CUTIL_OPTIMZE
#if defined(NDEBUG) || defined(_NDEBUG) || defined(RELEASE) || defined(_RELEASE)
#define CUTIL_OPTIMIZE 1
#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CUTIL_GCC 1
#else
#define CUTIL_GCC 0
#endif

#ifdef CUTIL_DONT_AUTO_CLEANUP_TYPES
#define CUTIL_AUTO_CLEANUP_TYPES 0
#define AUTO_CLEANUP(_cleanup)
#else
#if CUTIL_GCC
#define CUTIL_AUTO_CLEANUP_TYPES 1
#define CUTIL_AUTO_CLEANUP(_cleanup) __attribute__((cleanup(_cleanup)))
#else
#define CUTIL_AUTO_CLEANUP_TYPES 0
#define CUTIL_AUTO_CLEANUP(_cleanup)
#endif
#endif

#ifdef DEBUG
#define LOG(fmt, ...)                                                          \
  fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...) ((void)0)
#endif

#endif
