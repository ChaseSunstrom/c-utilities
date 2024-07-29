#include "error.h"

char *formatted_string(const char *p_format, ...) {
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

char *Error_T_to_string(Error_T variant) {
  switch (variant) {
  case ERROR_ALLOCATION_FAILED:
    return "Allocation failed";
  case ERROR_INVALID_ARGUMENT:
    return "Invalid argument";
  case ERROR_INVALID_OPERATION:
    return "Invalid operation";
  case ERROR_INVALID_STATE:
    return "Invalid state";
  case ERROR_NOT_IMPLEMENTED:
    return "Not implemented";
  case ERROR_OUT_OF_BOUNDS:
    return "Out of bounds";
  case ERROR_OUT_OF_MEMORY:
    return "Out of memory";
  case ERROR_UNKNOWN:
  default:
    return "Unknown error";
  }
}

char *Error_to_string(Error *p_error) {

#define MAX_MESSAGE_LENGTH 100

  char *p_message = p_error->p_message;
  char *p_variant = Error_T_to_string(p_error->variant);
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
      Error error = {.variant = ERROR_INVALID_ARGUMENT,
                     .p_message = "Error message too long"};
      return _Error_to_string(&error);
    }
    p_result[u_variant_length + 2 + i] = p_message[i];
  }

  p_result[u_total_length] = '\0';
  memset(&p_result[u_total_length], 0, MAX_MESSAGE_LENGTH - u_total_length);

  Error_free(p_error);

  return p_result;
}

Error *Error_new(Error_T variant, char *p_message, bool b_alloc) {
  Error *error = malloc(sizeof(Error));
  error->variant = variant;
  error->p_message = p_message;
  error->b_message_allocated = b_alloc;
  return error;
}

void Error_free(Error *p_error) {
  if (!p_error) {
    return;
  }
  if (p_error->b_message_allocated) {
    free(p_error->p_message);
  }
  free(p_error);
  p_error = NULL;
}
