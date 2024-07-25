#include "result.h"

void *Result_or_else(_Result_T result, void *(*p_fn)(_Error_T *)) {
  if (Result_is_err(result)) {
    return p_fn(result.p_error);
  }

  _Error_free(result.p_error);

  return result.p_data;
}

void *Result_expect(_Result_T result, char *p_message) {
  if (Result_is_err(result)) {
    printf("%s", p_message);
    exit(EXIT_FAILURE);
  }

  _Error_free(result.p_error);

  return result.p_data;
}

void *Result_unwrap(_Result_T result) {
  if (Result_is_err(result)) {
    printf("Error: %s", _Error_to_string(result.p_error));
    exit(EXIT_FAILURE);
  }

  _Error_free(result.p_error);

  return result.p_data;
}
