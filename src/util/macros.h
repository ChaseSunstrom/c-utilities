#ifndef CUTIL_MACROS_H
#define CUTIL_MACROS_H

#define EXPAND(x) x
#define APPLY(macro, ...) EXPAND(macro(__VA_ARGS__))
#define ARG1(a, ...) a
#define ARG2(a, b, ...) b
#define ARG3(a, b, c, ...) c
#define ARG4(a, b, c, d, ...) d
#define ARGS2(a, b, ...) a, b
#define ARGS3(a, b, c, ...) a, b, c
#define ARGS4(a, b, c, d, ...) a, b, c, d

#define FOR_EACH_1(what, x, ...) what(x)
#define FOR_EACH_2(what, x, ...) what(x) EXPAND(FOR_EACH_1(what, __VA_ARGS__))
#define FOR_EACH_3(what, x, ...) what(x) EXPAND(FOR_EACH_2(what, __VA_ARGS__))
#define FOR_EACH_4(what, x, ...) what(x) EXPAND(FOR_EACH_3(what, __VA_ARGS__))
#define FOR_EACH_5(what, x, ...) what(x) EXPAND(FOR_EACH_4(what, __VA_ARGS__))

#define IF_ARGS_ELSE(args, if_args, if_empty)                                  \
  CONCAT(IF_ARGS_ELSE_, NARGS(args))(if_args, if_empty)
#define IF_ARGS_ELSE_0(if_args, if_empty) if_empty
#define IF_ARGS_ELSE_1(if_args, if_empty) if_args
#define IF_ARGS_ELSE_2(if_args, if_empty) if_args
#define IF_ARGS_ELSE_3(if_args, if_empty) if_args
#define IF_ARGS_ELSE_4(if_args, if_empty) if_args
#define IF_ARGS_ELSE_5(if_args, if_empty) if_args
#define IF_ARGS_ELSE_6(if_args, if_empty) if_args
#define IF_ARGS_ELSE_7(if_args, if_empty) if_args
#define IF_ARGS_ELSE_8(if_args, if_empty) if_args

// Macro magic for overloading
#define NARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b

#define FREE_THEN_NULL(ptr)                                                    \
  do {                                                                         \
    if (ptr) {                                                                 \
      free(ptr);                                                               \
      ptr = NULL;                                                              \
    }                                                                          \
  } while (0)

#endif
