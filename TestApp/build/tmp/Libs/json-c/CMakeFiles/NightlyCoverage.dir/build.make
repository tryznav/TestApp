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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.17.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.17.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tryznav/TestApp/TestApp/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tryznav/TestApp/TestApp/build/tmp

# Utility rule file for NightlyCoverage.

# Include the progress variables for this target.
include Libs/json-c/CMakeFiles/NightlyCoverage.dir/progress.make

Libs/json-c/CMakeFiles/NightlyCoverage:
	cd /Users/tryznav/TestApp/TestApp/build/tmp/Libs/json-c && /usr/local/Cellar/cmake/3.17.2/bin/ctest -D NightlyCoverage

NightlyCoverage: Libs/json-c/CMakeFiles/NightlyCoverage
NightlyCoverage: Libs/json-c/CMakeFiles/NightlyCoverage.dir/build.make

.PHONY : NightlyCoverage

# Rule to build all files generated by this target.
Libs/json-c/CMakeFiles/NightlyCoverage.dir/build: NightlyCoverage

.PHONY : Libs/json-c/CMakeFiles/NightlyCoverage.dir/build

Libs/json-c/CMakeFiles/NightlyCoverage.dir/clean:
	cd /Users/tryznav/TestApp/TestApp/build/tmp/Libs/json-c && $(CMAKE_COMMAND) -P CMakeFiles/NightlyCoverage.dir/cmake_clean.cmake
.PHONY : Libs/json-c/CMakeFiles/NightlyCoverage.dir/clean

Libs/json-c/CMakeFiles/NightlyCoverage.dir/depend:
	cd /Users/tryznav/TestApp/TestApp/build/tmp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tryznav/TestApp/TestApp/build /Users/tryznav/TestApp/TestApp/build/Libs/json-c /Users/tryznav/TestApp/TestApp/build/tmp /Users/tryznav/TestApp/TestApp/build/tmp/Libs/json-c /Users/tryznav/TestApp/TestApp/build/tmp/Libs/json-c/CMakeFiles/NightlyCoverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Libs/json-c/CMakeFiles/NightlyCoverage.dir/depend
