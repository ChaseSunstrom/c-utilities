#include "string.h"

String* String_default() {
  return String_new_1(NULL);
}

String *String_new_1(Allocator *p_allocator) {
  return String_new_3(NULL, 0, p_allocator);
}

String *String_new_2(const char *p_str, Allocator *p_allocator) {
  return String_new_3(p_str, strlen(p_str), p_allocator);
}

String *String_new_3(const char *p_str, size_t u_size, Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  String *p_string = Allocator_alloc(p_allocator, sizeof(String));
  if (!p_string) {
    return NULL;
  }

  p_string->p_data = Allocator_alloc(p_allocator, u_size + 1);
  if (!p_string->p_data) {
    Allocator_free(p_allocator, p_string);
    return NULL;
  }

  if (p_str) {
    memcpy(p_string->p_data, p_str, u_size);
  }
  p_string->p_data[u_size] = '\0';
  p_string->u_capacity = u_size;
  p_string->u_size = u_size;
  p_string->p_allocator = p_allocator;
  p_string->b_external_allocator = b_external_allocator;

  return p_string;
}

void String_free(String *p_string) {
  if (!p_string) {
    return;
  }

  Allocator *p_allocator = p_string->p_allocator;
  if (!p_allocator) {
    return;
  }

  Allocator_free(p_allocator, p_string->p_data);
  Allocator_free(p_allocator, p_string);
}

void String_append(String *p_string, const char *p_str) {
  if (!p_string || !p_str) {
    return;
  }

  size_t u_size = strlen(p_str);
  if (u_size == 0) {
    return;
  }

  size_t u_new_size = p_string->u_size + u_size;
  if (u_new_size >= p_string->u_capacity) {
    size_t u_capacity = p_string->u_capacity * 2;
    while (u_new_size >= u_capacity) {
      u_capacity *= 2;
    }

    char *p_data = Allocator_alloc(p_string->p_allocator, u_capacity + 1);
    if (!p_data) {
      return;
    }

    memcpy(p_data, p_string->p_data, p_string->u_size);
    Allocator_free(p_string->p_allocator, p_string->p_data);
    p_string->p_data = p_data;
    p_string->u_capacity = u_capacity;
  }

  memcpy(p_string->p_data + p_string->u_size, p_str, u_size);
  p_string->u_size = u_new_size;
  p_string->p_data[p_string->u_size] = '\0';
}

void String_clear(String *p_string) {
  if (!p_string) {
    return;
  }

  p_string->u_size = 0;
  p_string->p_data[0] = '\0';
}

bool String_empty(String *p_string) {
  return p_string ? p_string->u_size == 0 : true;
}

void String_insert(String *p_string, size_t u_index, const char *p_str) {
  if (!p_string || !p_str || u_index >= p_string->u_size) {
    return;
  }

  size_t u_size = strlen(p_str);
  if (u_size == 0) {
    return;
  }

  size_t u_new_size = p_string->u_size + u_size;
  if (u_new_size >= p_string->u_capacity) {
    size_t u_capacity = p_string->u_capacity * 2;
    while (u_new_size >= u_capacity) {
      u_capacity *= 2;
    }

    char *p_data = Allocator_alloc(p_string->p_allocator, u_capacity + 1);
    if (!p_data) {
      return;
    }

    memcpy(p_data, p_string->p_data, u_index);
    memcpy(p_data + u_index, p_str, u_size);
    memcpy(p_data + u_index + u_size, p_string->p_data + u_index,
           p_string->u_size - u_index);
    Allocator_free(p_string->p_allocator, p_string->p_data);
    p_string->p_data = p_data;
    p_string->u_capacity = u_capacity;
  } else {
    memmove(p_string->p_data + u_index + u_size, p_string->p_data + u_index,
            p_string->u_size - u_index);
    memcpy(p_string->p_data + u_index, p_str, u_size);
  }

  p_string->u_size = u_new_size;
  p_string->p_data[p_string->u_size] = '\0';
}

void String_erase(String *p_string, size_t u_index, size_t u_size) {
  if (!p_string || u_index >= p_string->u_size) {
    return;
  }

  if (u_index + u_size >= p_string->u_size) {
    p_string->u_size = u_index;
    p_string->p_data[p_string->u_size] = '\0';
    return;
  }

  memmove(p_string->p_data + u_index, p_string->p_data + u_index + u_size,
          p_string->u_size - u_index - u_size);
  p_string->u_size -= u_size;
  p_string->p_data[p_string->u_size] = '\0';
}

void String_replace(String *p_string, const char *p_old, const char *p_new) {
  if (!p_string || !p_old || !p_new) {
    return;
  }

  size_t u_old_size = strlen(p_old);
  if (u_old_size == 0) {
    return;
  }

  size_t u_new_size = strlen(p_new);
  if (u_new_size == 0) {
    return;
  }

  size_t u_index = 0;
  while ((u_index = String_find(p_string, p_old, &u_index)) != SIZE_MAX) {
    String_erase(p_string, u_index, u_old_size);
    String_insert(p_string, u_index, p_new);
    u_index += u_new_size;
  }
}

void String_replace_all(String *p_string, const char *p_old, const char *p_new) {
  if (!p_string || !p_old || !p_new) {
    return;
  }

  size_t u_old_size = strlen(p_old);
  if (u_old_size == 0) {
    return;
  }

  size_t u_new_size = strlen(p_new);
  if (u_new_size == 0) {
    return;
  }

  size_t u_index = 0;
  while ((u_index = String_find(p_string, p_old, &u_index)) != SIZE_MAX) {
    String_erase(p_string, u_index, u_old_size);
    String_insert(p_string, u_index, p_new);
    u_index += u_new_size;
  }
}

size_t String_find(String *p_string, const char *p_str, size_t *p_index) {
  if (!p_string || !p_str) {
    return SIZE_MAX;
  }

  size_t u_size = strlen(p_str);
  if (u_size == 0) {
    return SIZE_MAX;
  }

  size_t u_index = p_index ? *p_index : 0;
  if (u_index >= p_string->u_size) {
    return SIZE_MAX;
  }

  for (size_t i = u_index; i < p_string->u_size; i++) {
    if (strncmp(p_string->p_data + i, p_str, u_size) == 0) {
      if (p_index) {
        *p_index = i + u_size;
      }
      return i;
    }
  }

  return SIZE_MAX;
}

void String_trim(String *p_string) {
  if (!p_string) {
    return;
  }

  String_trim_left(p_string);
  String_trim_right(p_string);
}

void String_trim_left(String *p_string) {
  if (!p_string) {
    return;
  }

  size_t u_index = 0;
  while (u_index < p_string->u_size && isspace(p_string->p_data[u_index])) {
    u_index++;
  }

  if (u_index > 0) {
    String_erase(p_string, 0, u_index);
  }
}

void String_trim_right(String *p_string) {
  if (!p_string) {
    return;
  }

  size_t u_index = p_string->u_size;
  while (u_index > 0 && isspace(p_string->p_data[u_index - 1])) {
    u_index--;
  }

  if (u_index < p_string->u_size) {
    String_erase(p_string, u_index, p_string->u_size - u_index);
  }
}

void String_to_lower(String *p_string) {
  if (!p_string) {
    return;
  }

  for (size_t i = 0; i < p_string->u_size; i++) {
    p_string->p_data[i] = tolower(p_string->p_data[i]);
  }
}

void String_to_upper(String *p_string) {
  if (!p_string) {
    return;
  }

  for (size_t i = 0; i < p_string->u_size; i++) {
    p_string->p_data[i] = toupper(p_string->p_data[i]);
  }
}

void String_reverse(String *p_string) {
  if (!p_string) {
    return;
  }

  for (size_t i = 0; i < p_string->u_size / 2; i++) {
    char c = p_string->p_data[i];
    p_string->p_data[i] = p_string->p_data[p_string->u_size - i - 1];
    p_string->p_data[p_string->u_size - i - 1] = c;
  }
}
