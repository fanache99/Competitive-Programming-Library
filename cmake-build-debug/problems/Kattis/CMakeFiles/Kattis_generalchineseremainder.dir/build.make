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
include problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/depend.make

# Include the progress variables for this target.
include problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/progress.make

# Include the compile flags for this target's objects.
include problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/flags.make

problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.o: problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/flags.make
problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.o: ../problems/Kattis/generalchineseremainder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stefan/Desktop/CP/library/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.o"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis && /usr/local/Cellar/gcc/12.2.0/bin/g++-12  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.o -c /Users/stefan/Desktop/CP/library/problems/Kattis/generalchineseremainder.cpp

problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.i"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis && /usr/local/Cellar/gcc/12.2.0/bin/g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/stefan/Desktop/CP/library/problems/Kattis/generalchineseremainder.cpp > CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.i

problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.s"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis && /usr/local/Cellar/gcc/12.2.0/bin/g++-12 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/stefan/Desktop/CP/library/problems/Kattis/generalchineseremainder.cpp -o CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.s

# Object files for target Kattis_generalchineseremainder
Kattis_generalchineseremainder_OBJECTS = \
"CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.o"

# External object files for target Kattis_generalchineseremainder
Kattis_generalchineseremainder_EXTERNAL_OBJECTS =

problems/Kattis/Kattis_generalchineseremainder: problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/generalchineseremainder.cpp.o
problems/Kattis/Kattis_generalchineseremainder: problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/build.make
problems/Kattis/Kattis_generalchineseremainder: problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/stefan/Desktop/CP/library/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Kattis_generalchineseremainder"
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Kattis_generalchineseremainder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/build: problems/Kattis/Kattis_generalchineseremainder

.PHONY : problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/build

problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/clean:
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis && $(CMAKE_COMMAND) -P CMakeFiles/Kattis_generalchineseremainder.dir/cmake_clean.cmake
.PHONY : problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/clean

problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/depend:
	cd /Users/stefan/Desktop/CP/library/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/stefan/Desktop/CP/library /Users/stefan/Desktop/CP/library/problems/Kattis /Users/stefan/Desktop/CP/library/cmake-build-debug /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis /Users/stefan/Desktop/CP/library/cmake-build-debug/problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : problems/Kattis/CMakeFiles/Kattis_generalchineseremainder.dir/depend

