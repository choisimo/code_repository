# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /home/nodove/.cache/JetBrains/RemoteDev/dist/1729cdbf0ed8b_CLion-2024.2.2/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/nodove/.cache/JetBrains/RemoteDev/dist/1729cdbf0ed8b_CLion-2024.2.2/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace/code_repository/cLang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/code_repository/cLang/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/new_target1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/new_target1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/new_target1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/new_target1.dir/flags.make

CMakeFiles/new_target1.dir/1030/infinite_loop.c.o: CMakeFiles/new_target1.dir/flags.make
CMakeFiles/new_target1.dir/1030/infinite_loop.c.o: /workspace/code_repository/cLang/1030/infinite_loop.c
CMakeFiles/new_target1.dir/1030/infinite_loop.c.o: CMakeFiles/new_target1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/workspace/code_repository/cLang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/new_target1.dir/1030/infinite_loop.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/new_target1.dir/1030/infinite_loop.c.o -MF CMakeFiles/new_target1.dir/1030/infinite_loop.c.o.d -o CMakeFiles/new_target1.dir/1030/infinite_loop.c.o -c /workspace/code_repository/cLang/1030/infinite_loop.c

CMakeFiles/new_target1.dir/1030/infinite_loop.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/new_target1.dir/1030/infinite_loop.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/code_repository/cLang/1030/infinite_loop.c > CMakeFiles/new_target1.dir/1030/infinite_loop.c.i

CMakeFiles/new_target1.dir/1030/infinite_loop.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/new_target1.dir/1030/infinite_loop.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/code_repository/cLang/1030/infinite_loop.c -o CMakeFiles/new_target1.dir/1030/infinite_loop.c.s

# Object files for target new_target1
new_target1_OBJECTS = \
"CMakeFiles/new_target1.dir/1030/infinite_loop.c.o"

# External object files for target new_target1
new_target1_EXTERNAL_OBJECTS =

new_target1: CMakeFiles/new_target1.dir/1030/infinite_loop.c.o
new_target1: CMakeFiles/new_target1.dir/build.make
new_target1: CMakeFiles/new_target1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/workspace/code_repository/cLang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable new_target1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/new_target1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/new_target1.dir/build: new_target1
.PHONY : CMakeFiles/new_target1.dir/build

CMakeFiles/new_target1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/new_target1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/new_target1.dir/clean

CMakeFiles/new_target1.dir/depend:
	cd /workspace/code_repository/cLang/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/code_repository/cLang /workspace/code_repository/cLang /workspace/code_repository/cLang/cmake-build-debug /workspace/code_repository/cLang/cmake-build-debug /workspace/code_repository/cLang/cmake-build-debug/CMakeFiles/new_target1.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/new_target1.dir/depend
