# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\nodove\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\nodove\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\git\code_repository

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\git\code_repository\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cLang++_report_ch4_exam_10.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cLang++_report_ch4_exam_10.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cLang++_report_ch4_exam_10.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cLang++_report_ch4_exam_10.dir/flags.make

CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj: CMakeFiles/cLang++_report_ch4_exam_10.dir/flags.make
CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj: C:/git/code_repository/cLang++/final/1107/Person.cpp
CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj: CMakeFiles/cLang++_report_ch4_exam_10.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\git\code_repository\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj"
	C:\Users\nodove\AppData\Local\Programs\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj -MF CMakeFiles\cLang++_report_ch4_exam_10.dir\cLang++\final\1107\Person.cpp.obj.d -o CMakeFiles\cLang++_report_ch4_exam_10.dir\cLang++\final\1107\Person.cpp.obj -c C:\git\code_repository\cLang++\final\1107\Person.cpp

CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.i"
	C:\Users\nodove\AppData\Local\Programs\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\git\code_repository\cLang++\final\1107\Person.cpp > CMakeFiles\cLang++_report_ch4_exam_10.dir\cLang++\final\1107\Person.cpp.i

CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.s"
	C:\Users\nodove\AppData\Local\Programs\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\git\code_repository\cLang++\final\1107\Person.cpp -o CMakeFiles\cLang++_report_ch4_exam_10.dir\cLang++\final\1107\Person.cpp.s

# Object files for target cLang++_report_ch4_exam_10
cLang_______report_ch4_exam_10_OBJECTS = \
"CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj"

# External object files for target cLang++_report_ch4_exam_10
cLang_______report_ch4_exam_10_EXTERNAL_OBJECTS =

cLang++_report_ch4_exam_10.exe: CMakeFiles/cLang++_report_ch4_exam_10.dir/cLang++/final/1107/Person.cpp.obj
cLang++_report_ch4_exam_10.exe: CMakeFiles/cLang++_report_ch4_exam_10.dir/build.make
cLang++_report_ch4_exam_10.exe: CMakeFiles/cLang++_report_ch4_exam_10.dir/linkLibs.rsp
cLang++_report_ch4_exam_10.exe: CMakeFiles/cLang++_report_ch4_exam_10.dir/objects1.rsp
cLang++_report_ch4_exam_10.exe: CMakeFiles/cLang++_report_ch4_exam_10.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\git\code_repository\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cLang++_report_ch4_exam_10.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\cLang++_report_ch4_exam_10.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cLang++_report_ch4_exam_10.dir/build: cLang++_report_ch4_exam_10.exe
.PHONY : CMakeFiles/cLang++_report_ch4_exam_10.dir/build

CMakeFiles/cLang++_report_ch4_exam_10.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\cLang++_report_ch4_exam_10.dir\cmake_clean.cmake
.PHONY : CMakeFiles/cLang++_report_ch4_exam_10.dir/clean

CMakeFiles/cLang++_report_ch4_exam_10.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\git\code_repository C:\git\code_repository C:\git\code_repository\cmake-build-debug C:\git\code_repository\cmake-build-debug C:\git\code_repository\cmake-build-debug\CMakeFiles\cLang++_report_ch4_exam_10.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cLang++_report_ch4_exam_10.dir/depend
