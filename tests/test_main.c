#include "test_main.h"
#include "test_list.h"
#include "test_vector.h"

static _Test_T *_Test_new(char *p_name, bool (*p_fn)()) {
  _Test_T *p_test = malloc(sizeof(_Test_T));
  p_test->p_name = p_name;
  p_test->p_fn = p_fn;
  return p_test;
}

void _Test_free(void *p_test) { free(p_test); }

_Tests_T *_Tests_T_new() {
  _Tests_T *p_tests = malloc(sizeof(_Tests_T));
  p_tests->v_tests = Vector_new(_Test_free, NULL);
  return p_tests;
}

void _Tests_T_add(_Tests_T *p_tests, char *p_name, bool (*p_fn)()) {
  Vector_push_back(p_tests->v_tests, (void *)_Test_new(p_name, p_fn));
}

void _Tests_T_run(_Tests_T *p_tests) {

  _Tests_T_add(p_tests, "test_vector_new", test_vector_new);
  _Tests_T_add(p_tests, "test_vector_push_back", test_vector_push_back);
  _Tests_T_add(p_tests, "test_vector_at", test_vector_at);
  _Tests_T_add(p_tests, "test_vector_front", test_vector_front);
  _Tests_T_add(p_tests, "test_vector_back", test_vector_back);
  _Tests_T_add(p_tests, "test_vector_pop_back", test_vector_pop_back);
  _Tests_T_add(p_tests, "test_vector_insert", test_vector_insert);
  _Tests_T_add(p_tests, "test_vector_erase", test_vector_erase);
  _Tests_T_add(p_tests, "test_vector_clear", test_vector_clear);

  _Tests_T_add(p_tests, "test_list_new", test_list_new);
  _Tests_T_add(p_tests, "test_list_push_back", test_list_push_back);
  _Tests_T_add(p_tests, "test_list_pop_back", test_list_pop_back);
  _Tests_T_add(p_tests, "test_list_push_front", test_list_push_front);
  _Tests_T_add(p_tests, "test_list_pop_front", test_list_pop_front);
  _Tests_T_add(p_tests, "test_list_remove", test_list_remove);
  _Tests_T_add(p_tests, "test_list_insert", test_list_insert);
  _Tests_T_add(p_tests, "test_list_erase", test_list_erase);
  _Tests_T_add(p_tests, "test_list_at", test_list_at);
  _Tests_T_add(p_tests, "test_list_front", test_list_front);
  _Tests_T_add(p_tests, "test_list_back", test_list_back);
  _Tests_T_add(p_tests, "test_list_empty", test_list_empty);

  printf("Running tests...\n");

  for (size_t i = 0; i < p_tests->v_tests->container->u_size; i++) {
    _Test_T *p_test = (_Test_T *)Vector_at(p_tests->v_tests, i);
    if (p_test->p_fn()) {
      printf("%s passed \n", p_test->p_name);
    } else {
      printf("%s failed\n", p_test->p_name);
    }
  }

  Vector_free(p_tests->v_tests);
  free(p_tests);
}
