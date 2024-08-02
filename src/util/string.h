#ifndef CUTIL_STRING_H
#define CUTIL_STRING_H

#include "alloc.h"

typedef struct {
  char *p_data;
  size_t u_capacity;
  size_t u_size;
  Allocator *p_allocator;
  bool b_external_allocator;
} String;

#define String_new(...) CONCAT(String_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

String *String_default();
String *String_new_1(Allocator *p_allocator);
String *String_new_2(const char *p_str, Allocator *p_allocator);
String *String_new_3(const char *p_str, size_t u_size, Allocator *p_allocator);
void String_free(String *p_string);
void String_free_(String **pp_string);
void String_append(String *p_string, const char *p_str);
void String_clear(String *p_string);
bool String_empty(String *p_string);
void String_insert(String *p_string, size_t u_index, const char *p_str);
void String_erase(String *p_string, size_t u_index, size_t u_size);
void String_replace(String *p_string, const char *p_old, const char *p_new);
void String_replace_all(String *p_string, const char *p_old, const char *p_new);
void String_trim(String *p_string);
void String_trim_left(String *p_string);
void String_trim_right(String *p_string);
void String_to_lower(String *p_string);
void String_to_upper(String *p_string);
void String_reverse(String *p_string);
size_t String_find(String *p_string, const char *p_str, size_t *p_index);

#endif
