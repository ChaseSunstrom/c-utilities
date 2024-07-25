#ifndef SPECTRA_RESULT_H
#define SPECTRA_RESULT_H

#include "error.h"

typedef struct {
  bool b_is_ok;
  _Error_T *p_error;
  void *p_data;
} _Result_T;

#define Result_T(type) _Result_T

#define Result_new_ok(type, data)                                              \
  (Result_T(type)) { .b_is_ok = true, .p_error = NULL, .p_data = (void *)data }

#define Result_new_err(type, error)                                            \
  (Result_T(type)) { .b_is_ok = false, .p_error = error, .p_data = NULL }

#define Result_is_ok(result) (result.b_is_ok)

#define Result_is_err(result) (!result.b_is_ok)

void *Result_or_else(_Result_T result, void *(*p_fn)(_Error_T *));
void *Result_expect(_Result_T result, char *p_message);
void *Result_unwrap(_Result_T result);

#endif
