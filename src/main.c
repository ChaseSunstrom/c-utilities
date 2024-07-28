#include "../tests/test_main.h"
#define CUTIL_TEST
i32 main() {
#ifdef CUTIL_TEST
  _Tests_T *p_tests = _Tests_T_new();
  _Tests_T_run(p_tests);
#endif
  return 0;
}
