#include "../tests/test_main.h"
#include "util/system.h"
#include <time.h>
// #define CUTIL_TEST
#include "util/hash.h"
#include "util/unordered_map.h"

i32 main() {
#ifdef CUTIL_TEST
  _Tests_T *p_tests = _Tests_T_new();
  _Tests_T_run(p_tests);
#endif

  Unordered_Map(const char *, const char *) p_map = Unordered_Map_new(NULL);
  Unordered_Map_insert(p_map, "hello", "world");
  Unordered_Map_insert(p_map, "foo", "bar");
  Unordered_Map_insert(p_map, "baz", "qux");
  Unordered_Map_insert(p_map, "quux", "corge");
  Unordered_Map_insert(p_map, "grault", "garply");

  printf("hello: %s\n", Unordered_Map_at(p_map, "hello"));
  printf("foo: %s\n", Unordered_Map_at(p_map, "foo"));
  printf("baz: %s\n", Unordered_Map_at(p_map, "baz"));
  printf("quux: %s\n", Unordered_Map_at(p_map, "quux"));
  printf("grault: %s\n", Unordered_Map_at(p_map, "grault"));

  return 0;
}
