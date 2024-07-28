#ifndef CUTIL_TEST_MAIN_H
#define CUTIL_TEST_MAIN_H

#include "../src/util/pch.h"
#include "../src/util/vector.h"

typedef struct {
  char *p_name;
  bool (*p_fn)();
} _Test_T;

typedef struct {
  Vector(_Test_T) v_tests;
} _Tests_T;

_Tests_T *_Tests_T_new();
void _Tests_T_add(_Tests_T *p_tests, char *p_name, bool (*p_fn)());
void _Tests_T_run(_Tests_T *p_tests);

#endif
