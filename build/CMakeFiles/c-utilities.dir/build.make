# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chase/Projects/C/Spectra

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chase/Projects/C/Spectra/build

# Include any dependencies generated for this target.
include CMakeFiles/c-utilities.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/c-utilities.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/c-utilities.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/c-utilities.dir/flags.make

CMakeFiles/c-utilities.dir/src/main.c.o: CMakeFiles/c-utilities.dir/flags.make
CMakeFiles/c-utilities.dir/src/main.c.o: /home/chase/Projects/C/Spectra/src/main.c
CMakeFiles/c-utilities.dir/src/main.c.o: CMakeFiles/c-utilities.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chase/Projects/C/Spectra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/c-utilities.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-utilities.dir/src/main.c.o -MF CMakeFiles/c-utilities.dir/src/main.c.o.d -o CMakeFiles/c-utilities.dir/src/main.c.o -c /home/chase/Projects/C/Spectra/src/main.c

CMakeFiles/c-utilities.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/c-utilities.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chase/Projects/C/Spectra/src/main.c > CMakeFiles/c-utilities.dir/src/main.c.i

CMakeFiles/c-utilities.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/c-utilities.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chase/Projects/C/Spectra/src/main.c -o CMakeFiles/c-utilities.dir/src/main.c.s

CMakeFiles/c-utilities.dir/src/util/vector.c.o: CMakeFiles/c-utilities.dir/flags.make
CMakeFiles/c-utilities.dir/src/util/vector.c.o: /home/chase/Projects/C/Spectra/src/util/vector.c
CMakeFiles/c-utilities.dir/src/util/vector.c.o: CMakeFiles/c-utilities.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chase/Projects/C/Spectra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/c-utilities.dir/src/util/vector.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-utilities.dir/src/util/vector.c.o -MF CMakeFiles/c-utilities.dir/src/util/vector.c.o.d -o CMakeFiles/c-utilities.dir/src/util/vector.c.o -c /home/chase/Projects/C/Spectra/src/util/vector.c

CMakeFiles/c-utilities.dir/src/util/vector.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/c-utilities.dir/src/util/vector.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chase/Projects/C/Spectra/src/util/vector.c > CMakeFiles/c-utilities.dir/src/util/vector.c.i

CMakeFiles/c-utilities.dir/src/util/vector.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/c-utilities.dir/src/util/vector.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chase/Projects/C/Spectra/src/util/vector.c -o CMakeFiles/c-utilities.dir/src/util/vector.c.s

CMakeFiles/c-utilities.dir/src/util/error.c.o: CMakeFiles/c-utilities.dir/flags.make
CMakeFiles/c-utilities.dir/src/util/error.c.o: /home/chase/Projects/C/Spectra/src/util/error.c
CMakeFiles/c-utilities.dir/src/util/error.c.o: CMakeFiles/c-utilities.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chase/Projects/C/Spectra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/c-utilities.dir/src/util/error.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-utilities.dir/src/util/error.c.o -MF CMakeFiles/c-utilities.dir/src/util/error.c.o.d -o CMakeFiles/c-utilities.dir/src/util/error.c.o -c /home/chase/Projects/C/Spectra/src/util/error.c

CMakeFiles/c-utilities.dir/src/util/error.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/c-utilities.dir/src/util/error.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chase/Projects/C/Spectra/src/util/error.c > CMakeFiles/c-utilities.dir/src/util/error.c.i

CMakeFiles/c-utilities.dir/src/util/error.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/c-utilities.dir/src/util/error.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chase/Projects/C/Spectra/src/util/error.c -o CMakeFiles/c-utilities.dir/src/util/error.c.s

CMakeFiles/c-utilities.dir/src/util/result.c.o: CMakeFiles/c-utilities.dir/flags.make
CMakeFiles/c-utilities.dir/src/util/result.c.o: /home/chase/Projects/C/Spectra/src/util/result.c
CMakeFiles/c-utilities.dir/src/util/result.c.o: CMakeFiles/c-utilities.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chase/Projects/C/Spectra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/c-utilities.dir/src/util/result.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-utilities.dir/src/util/result.c.o -MF CMakeFiles/c-utilities.dir/src/util/result.c.o.d -o CMakeFiles/c-utilities.dir/src/util/result.c.o -c /home/chase/Projects/C/Spectra/src/util/result.c

CMakeFiles/c-utilities.dir/src/util/result.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/c-utilities.dir/src/util/result.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chase/Projects/C/Spectra/src/util/result.c > CMakeFiles/c-utilities.dir/src/util/result.c.i

CMakeFiles/c-utilities.dir/src/util/result.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/c-utilities.dir/src/util/result.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chase/Projects/C/Spectra/src/util/result.c -o CMakeFiles/c-utilities.dir/src/util/result.c.s

# Object files for target c-utilities
c__utilities_OBJECTS = \
"CMakeFiles/c-utilities.dir/src/main.c.o" \
"CMakeFiles/c-utilities.dir/src/util/vector.c.o" \
"CMakeFiles/c-utilities.dir/src/util/error.c.o" \
"CMakeFiles/c-utilities.dir/src/util/result.c.o"

# External object files for target c-utilities
c__utilities_EXTERNAL_OBJECTS =

c-utilities: CMakeFiles/c-utilities.dir/src/main.c.o
c-utilities: CMakeFiles/c-utilities.dir/src/util/vector.c.o
c-utilities: CMakeFiles/c-utilities.dir/src/util/error.c.o
c-utilities: CMakeFiles/c-utilities.dir/src/util/result.c.o
c-utilities: CMakeFiles/c-utilities.dir/build.make
c-utilities: CMakeFiles/c-utilities.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chase/Projects/C/Spectra/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable c-utilities"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/c-utilities.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/c-utilities.dir/build: c-utilities
.PHONY : CMakeFiles/c-utilities.dir/build

CMakeFiles/c-utilities.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/c-utilities.dir/cmake_clean.cmake
.PHONY : CMakeFiles/c-utilities.dir/clean

CMakeFiles/c-utilities.dir/depend:
	cd /home/chase/Projects/C/Spectra/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chase/Projects/C/Spectra /home/chase/Projects/C/Spectra /home/chase/Projects/C/Spectra/build /home/chase/Projects/C/Spectra/build /home/chase/Projects/C/Spectra/build/CMakeFiles/c-utilities.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/c-utilities.dir/depend

