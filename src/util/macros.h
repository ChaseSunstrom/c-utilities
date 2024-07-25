#ifndef SPECTRA_MACROS_H
#define SPECTRA_MACROS_H

#define EXPAND(x) x
#define APPLY(macro, ...) EXPAND(macro(__VA_ARGS__))
#define ARG1(a, ...) a
#define ARG2(a, b, ...) b
#define ARG3(a, b, c, ...) c
#define ARG4(a, b, c, d, ...) d
#define ARGS2(a, b, ...) a, b
#define ARGS3(a, b, c, ...) a, b, c
#define ARGS4(a, b, c, d, ...) a, b, c, d

// Concatenation macros
#define CAT(a, b) a##b
#define CAT3(a, b, c) a##b##c
#define CAT4(a, b, c, d) a##b##c##d

// Overloading macros
// Overloading macros
#define OVERLOAD1(name, a) name##_1(a)
#define OVERLOAD2(name, a, b) name##_2(a, b)
#define OVERLOAD3(name, a, b, c) name##_3(a, b, c)
#define OVERLOAD4(name, a, b, c, d) name##_4(a, b, c, d)

// Overload chooser
#define OVERLOAD_CHOOSER(_1, _2, _3, _4, NAME, ...) NAME

#endif
