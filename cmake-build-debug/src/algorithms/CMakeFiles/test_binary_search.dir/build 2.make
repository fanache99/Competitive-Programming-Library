# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/stefan/Desktop/CP/library

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/stefan/Desktop/CP/library/cmake-build-debug

# Include any dependencies generated for this target.
include src/algorithms/CMakeFiles/test_binary_search.dir/depend.make

# Include the progress variables for this target.
include src/algorithms/CMakeFiles/test_binary_search.dir/progress.make

# Include the compile flags for this target's objects.
include src/algorithms/CMakeFiles/test_binary_search.dir/flags.make

src/algorithms/CMakeFiles/test_binary_search.dir/test.cpp.o: src/algorithms/CMakeFiles/test_binary_search.dir/flags.make
src/algorithms/CMakeFiles/test_binary_search.dir/test.cpp.o: ../src/algorithms/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stefan/Desktop/CP/library/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/algorithms/CMakeFiles/test_binary_search.dir/test.cpp.o"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms && /usr/local/bin/g++-12  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_binary_search.dir/test.cpp.o -c /Users/stefan/Desktop/CP/library/src/algorithms/test.cpp

src/algorithms/CMakeFiles/test_binary_search.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_binary_search.dir/test.cpp.i"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms && /usr/local/bin/g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/stefan/Desktop/CP/library/src/algorithms/test.cpp > CMakeFiles/test_binary_search.dir/test.cpp.i

src/algorithms/CMakeFiles/test_binary_search.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_binary_search.dir/test.cpp.s"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms && /usr/local/bin/g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/stefan/Desktop/CP/library/src/algorithms/test.cpp -o CMakeFiles/test_binary_search.dir/test.cpp.s

# Object files for target test_binary_search
test_binary_search_OBJECTS = \
"CMakeFiles/test_binary_search.dir/test.cpp.o"

# External object files for target test_binary_search
test_binary_search_EXTERNAL_OBJECTS =

src/algorithms/test_binary_search: src/algorithms/CMakeFiles/test_binary_search.dir/test.cpp.o
src/algorithms/test_binary_search: src/algorithms/CMakeFiles/test_binary_search.dir/build.make
src/algorithms/test_binary_search: Catch2/src/libCatch2Maind.a
src/algorithms/test_binary_search: Catch2/src/libCatch2d.a
src/algorithms/test_binary_search: src/algorithms/CMakeFiles/test_binary_search.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/stefan/Desktop/CP/library/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_binary_search"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_binary_search.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/algorithms/CMakeFiles/test_binary_search.dir/build: src/algorithms/test_binary_search

.PHONY : src/algorithms/CMakeFiles/test_binary_search.dir/build

src/algorithms/CMakeFiles/test_binary_search.dir/clean:
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms && $(CMAKE_COMMAND) -P CMakeFiles/test_binary_search.dir/cmake_clean.cmake
.PHONY : src/algorithms/CMakeFiles/test_binary_search.dir/clean

src/algorithms/CMakeFiles/test_binary_search.dir/depend:
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/stefan/Desktop/CP/library /Users/stefan/Desktop/CP/library/src/algorithms /Users/stefan/Desktop/CP/library/cmake-build-debug /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms /Users/stefan/Desktop/CP/library/cmake-build-debug/src/algorithms/CMakeFiles/test_binary_search.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/algorithms/CMakeFiles/test_binary_search.dir/depend
