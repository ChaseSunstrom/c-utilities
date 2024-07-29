#ifndef CUTIL_RESULT_H
#define CUTIL_RESULT_H

#ifndef CUTIL_RESULT_INCLUDED
#define CUTIL_RESULT_INCLUDED
#endif

#include "error.h"

typedef struct {
  bool b_is_ok;
  Error *p_error;
  void *p_data;
} Result;

#define Result(type) Result

#define Result_new_ok(type, data)                                              \
  (Result(type)) { .b_is_ok = true, .p_error = NULL, .p_data = (void *)data }

#define Result_new_err(type, error)                                            \
  (Result(type)) { .b_is_ok = false, .p_error = error, .p_data = NULL }

#define Result_is_ok(result) (result.b_is_ok)

#define Result_is_err(result) (!result.b_is_ok)

void *Result_or_else(Result result, void *(*p_fn)(Error *));
void *Result_expect(Result result, char *p_message);
void *Result_unwrap(Result result);

#endif
