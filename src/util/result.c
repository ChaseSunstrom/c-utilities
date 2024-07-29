#include "result.h"

void *Result_or_else(Result result, void *(*p_fn)(Error *)) {
  if (Result_is_err(result)) {
    return p_fn(result.p_error);
  }

  Error_free(result.p_error);

  return result.p_data;
}

void *Result_expect(Result result, char *p_message) {
  if (Result_is_err(result)) {
    printf("%s", p_message);
    exit(EXIT_FAILURE);
  }

  Error_free(result.p_error);

  return result.p_data;
}

void *Result_unwrap(Result result) {
  if (Result_is_err(result)) {
    printf("Error: %s", Error_to_string(result.p_error));
    exit(EXIT_FAILURE);
  }

  Error_free(result.p_error);

  return result.p_data;
}
