cmake_minimum_required(VERSION 3.12)
project(c-utilities)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(
  ${PROJECT_NAME}
  src/main.c
  src/util/vector.c
  src/util/vector.h
  src/util/pch.h
  src/util/types.h
  src/util/error.c
  src/util/error.h
  src/util/result.c
  src/util/result.h
  src/util/mem.h
  src/util/mem.c
  src/util/system.h
  src/util/alloc.h
  src/util/alloc.c
  src/util/container.h
  src/util/container.c
  src/util/list.h
  src/util/list.c
  tests/test_main.h
  tests/test_main.c
  tests/test_vector.c
  tests/test_vector.h
  tests/test_list.h
  tests/test_list.c
  tests/test_allocator.c
  tests/test_allocator.h)

target_include_directories(${PROJECT_NAME} PRIVATE include)
