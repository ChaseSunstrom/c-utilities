#ifndef CUTIL_ERROR_H
#define CUTIL_ERROR_H

#ifndef CUTIL_ERROR_INCLUDED
#define CUTIL_ERROR_INCLUDED
#endif

#include "pch.h"

typedef enum {
  _ERROR_ALLOCATION_FAILED,
  _ERROR_INVALID_ARGUMENT,
  _ERROR_INVALID_OPERATION,
  _ERROR_INVALID_STATE,
  _ERROR_NOT_IMPLEMENTED,
  _ERROR_OUT_OF_BOUNDS,
  _ERROR_OUT_OF_MEMORY,
  _ERROR_UNKNOWN,
} _Error_Variant_T;

typedef struct {
  bool b_message_allocated;
  char *p_message;
  _Error_Variant_T variant;
} _Error_T;

char *_formatted_string(const char *p_format, ...);
char *_Error_Variant_T_to_string(_Error_Variant_T variant);
char *_Error_to_string(_Error_T *p_error);
_Error_T *_Error_new(_Error_Variant_T variant, char *p_message, bool b_alloc);
void _Error_free(_Error_T *p_error);

#define Error_to_string(p_error) _Error_to_string(p_error)

#endif
