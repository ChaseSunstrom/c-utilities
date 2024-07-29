#ifndef CUTIL_ERROR_H
#define CUTIL_ERROR_H

#ifndef CUTIL_ERROR_INCLUDED
#define CUTIL_ERROR_INCLUDED
#endif

#include "pch.h"

typedef enum {
  ERROR_ALLOCATION_FAILED,
  ERROR_INVALID_ARGUMENT,
  ERROR_INVALID_OPERATION,
  ERROR_INVALID_STATE,
  ERROR_NOT_IMPLEMENTED,
  ERROR_OUT_OF_BOUNDS,
  ERROR_OUT_OF_MEMORY,
  ERROR_UNKNOWN,
} Error_T;

typedef struct {
  bool b_message_allocated;
  char *p_message;
  Error_T variant;
} Error;

char *formatted_string(const char *p_format, ...);
char *Error_T_to_string(Error_T variant);
char *Error_to_string(Error *p_error);
Error *Error_new(Error_T variant, char *p_message, bool b_alloc);
void Error_free(Error *p_error);

#define Error_to_string(p_error) _Error_to_string(p_error)

#endif
