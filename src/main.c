#include "../tests/test_main.h"
#include "util/system.h"
#include <time.h>
// #define CUTIL_TEST

f64 get_time() { return (f64)clock() / CLOCKS_PER_SEC; }

void log_time(f64 start, f64 end, const char *message) {
  printf("Time: %f, Allocator: %s\n", end - start, message);
}

f64 test_allocator(Alloc_T alloc_type) {
  defer(Allocator_free_) Allocator *allocator = Allocator_new(alloc_type);
  Vector *v = Vector_new(allocator);
  f64 start = get_time();
  for (i64 i = 0; i < 100000000; i++) {
    Vector_push_back(v, i);
  }
  for (i64 i = 0; i < 100000000; i++) {
    v->container->p_data[i];
  }
  for (i64 i = 0; i < 100000000; i++) {
    Vector_pop_back(v);
  }
  f64 end = get_time();
  log_time(start, end, alloc_type == GENERAL_ALLOCATOR ? "General" : "Arena");
}

i32 main() {
#ifdef CUTIL_TEST
  _Tests_T *p_tests = _Tests_T_new();
  _Tests_T_run(p_tests);
#endif

  test_allocator(GENERAL_ALLOCATOR);

  test_allocator(ARENA_ALLOCATOR);

  return 0;
}
