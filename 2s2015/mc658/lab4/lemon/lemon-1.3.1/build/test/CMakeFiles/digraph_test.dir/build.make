# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build

# Include any dependencies generated for this target.
include test/CMakeFiles/digraph_test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/digraph_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/digraph_test.dir/flags.make

test/CMakeFiles/digraph_test.dir/digraph_test.cc.o: test/CMakeFiles/digraph_test.dir/flags.make
test/CMakeFiles/digraph_test.dir/digraph_test.cc.o: ../test/digraph_test.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object test/CMakeFiles/digraph_test.dir/digraph_test.cc.o"
	cd /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/digraph_test.dir/digraph_test.cc.o -c /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/test/digraph_test.cc

test/CMakeFiles/digraph_test.dir/digraph_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/digraph_test.dir/digraph_test.cc.i"
	cd /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/test/digraph_test.cc > CMakeFiles/digraph_test.dir/digraph_test.cc.i

test/CMakeFiles/digraph_test.dir/digraph_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/digraph_test.dir/digraph_test.cc.s"
	cd /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/test/digraph_test.cc -o CMakeFiles/digraph_test.dir/digraph_test.cc.s

test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.requires:
.PHONY : test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.requires

test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.provides: test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/digraph_test.dir/build.make test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.provides.build
.PHONY : test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.provides

test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.provides.build: test/CMakeFiles/digraph_test.dir/digraph_test.cc.o

# Object files for target digraph_test
digraph_test_OBJECTS = \
"CMakeFiles/digraph_test.dir/digraph_test.cc.o"

# External object files for target digraph_test
digraph_test_EXTERNAL_OBJECTS =

test/digraph_test: test/CMakeFiles/digraph_test.dir/digraph_test.cc.o
test/digraph_test: test/CMakeFiles/digraph_test.dir/build.make
test/digraph_test: lemon/libemon.a
test/digraph_test: /usr/local/lib/libglpk.so
test/digraph_test: test/CMakeFiles/digraph_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable digraph_test"
	cd /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/digraph_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/digraph_test.dir/build: test/digraph_test
.PHONY : test/CMakeFiles/digraph_test.dir/build

test/CMakeFiles/digraph_test.dir/requires: test/CMakeFiles/digraph_test.dir/digraph_test.cc.o.requires
.PHONY : test/CMakeFiles/digraph_test.dir/requires

test/CMakeFiles/digraph_test.dir/clean:
	cd /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test && $(CMAKE_COMMAND) -P CMakeFiles/digraph_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/digraph_test.dir/clean

test/CMakeFiles/digraph_test.dir/depend:
	cd /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1 /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/test /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test/CMakeFiles/digraph_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/digraph_test.dir/depend

