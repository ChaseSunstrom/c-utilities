#include "error.h"

char *_formatted_string(const char *p_format, ...) {
  va_list args;
  va_list args_copy;
  va_start(args, p_format);
  va_copy(args_copy, args);

  // Get the required length
  i32 length = vsnprintf(NULL, 0, p_format, args);
  va_end(args);

  if (length < 0) {
    va_end(args_copy);
    return NULL; // Error in formatting
  }

  // Allocate memory
  char *p_result = malloc(length + 1);
  if (p_result == NULL) {
    va_end(args_copy);
    return NULL; // Memory allocation failed
  }

  // Actually format the string
  vsnprintf(p_result, length + 1, p_format, args_copy);
  va_end(args_copy);

  return p_result;
}

char *_Error_Variant_T_to_string(_Error_Variant_T variant) {
  switch (variant) {
  case _ERROR_ALLOCATION_FAILED:
    return "Allocation failed";
  case _ERROR_INVALID_ARGUMENT:
    return "Invalid argument";
  case _ERROR_INVALID_OPERATION:
    return "Invalid operation";
  case _ERROR_INVALID_STATE:
    return "Invalid state";
  case _ERROR_NOT_IMPLEMENTED:
    return "Not implemented";
  case _ERROR_OUT_OF_BOUNDS:
    return "Out of bounds";
  case _ERROR_OUT_OF_MEMORY:
    return "Out of memory";
  case _ERROR_UNKNOWN:
  default:
    return "Unknown error";
  }
}

char *_Error_to_string(_Error_T *p_error) {

#define MAX_MESSAGE_LENGTH 100

  char *p_message = p_error->p_message;
  char *p_variant = _Error_Variant_T_to_string(p_error->variant);
  u64 u_message_length = strlen(p_message);
  u64 u_variant_length = strlen(p_variant);

  u8 u_total_length = u_variant_length + 2 + u_message_length;

  static char p_result[MAX_MESSAGE_LENGTH] = "";

  for (u8 i = 0; i < u_variant_length; i++) {
    p_result[i] = p_variant[i];
  }

  p_result[u_variant_length] = ':';
  p_result[u_variant_length + 1] = ' ';

  for (u8 i = 0; i < u_message_length; i++) {
    // Minus 3 for the colon, space, and null terminator
    if (i >= MAX_MESSAGE_LENGTH - u_variant_length - 3) {
      // Break early here so we dont cause a buffer overflow
      _Error_T error = {.variant = _ERROR_INVALID_ARGUMENT,
                        .p_message = "Error message too long"};
      return _Error_to_string(&error);
    }
    p_result[u_variant_length + 2 + i] = p_message[i];
  }

  p_result[u_total_length] = '\0';
  memset(&p_result[u_total_length], 0, MAX_MESSAGE_LENGTH - u_total_length);

  _Error_free(p_error);

  return p_result;
}

_Error_T *_Error_new(_Error_Variant_T variant, char *p_message, bool b_alloc) {
  _Error_T *error = malloc(sizeof(_Error_T));
  error->variant = variant;
  error->p_message = p_message;
  error->b_message_allocated = b_alloc;
  return error;
}

void _Error_free(_Error_T *p_error) {
  if (!p_error) {
    return;
  }
  if (p_error->b_message_allocated) {
    free(p_error->p_message);
  }
  free(p_error);
  p_error = NULL;
}
