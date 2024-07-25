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
  src/util/result.h)
target_include_directories(${PROJECT_NAME} PRIVATE include)
