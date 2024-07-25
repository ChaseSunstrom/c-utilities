#include "util/vector.h"

static void *_print_error(_Error_T *p_error) {
  printf("%s", _Error_to_string(p_error));
  exit(EXIT_FAILURE);
}

i32 main() {
  Vector(i32) p_vector = _Vector_T_new(sizeof(char *));

  _Vector_T_push_back(p_vector, "Hello, World!");
  _Vector_T_push_back(p_vector, "Hi");

  // Fix the result freeing,
  // right now it does not take in stuff that would need to be freed in case of
  // failure
  void *p_elem = Result_or_else(_Vector_T_at(p_vector, 1), _print_error);

  printf("Element at index 1: %s\n", p_elem);

  _Vector_T_free(p_vector);

  return 0;
}
