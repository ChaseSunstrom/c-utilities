cmake_minimum_required(VERSION 3.12)
project(c-utilities)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Debugging output
message(STATUS "Project source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Main.c location: ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

# Ensure static linking
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

# Main library (now explicitly static)
add_library(c_utilities_lib STATIC
  src/util/vector.c
  src/util/hash.c
  src/util/unordered_map.c
  src/util/mem.c
  src/util/alloc.c
  src/util/container.c
  src/util/list.c
  src/util/tree.c
  src/util/string.c
  src/util/queue.c
  src/util/dequeue.c
  src/util/stack.c
)

target_include_directories(c_utilities_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main executable
add_executable(c_utilities_exe "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
set_target_properties(c_utilities_exe PROPERTIES OUTPUT_NAME "c-utilities")
target_link_libraries(c_utilities_exe PRIVATE c_utilities_lib)

# Test executable
add_executable(c_utilities_test
  tests/test_main.c
  tests/test_vector.c
  tests/test_list.c
  tests/test_allocator.c
)
target_link_libraries(c_utilities_test PRIVATE c_utilities_lib)

# Set startup targets
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT c_utilities_exe)

# Add debug symbols
if(MSVC)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
endif()

# Link math library only if it exists (for non-Windows platforms)
if(NOT WIN32)
  target_link_libraries(c_utilities_exe PRIVATE m)
  target_link_libraries(c_utilities_test PRIVATE m)
endif()

# More debugging output
message(STATUS "C_UTILITIES_EXE sources: $<TARGET_PROPERTY:c_utilities_exe,SOURCES>")