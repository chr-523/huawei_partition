# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/chr_523/Work_all/huawei_partition/main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chr_523/Work_all/huawei_partition/main/build

# Include any dependencies generated for this target.
include CMakeFiles/huawei_.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/huawei_.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/huawei_.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/huawei_.dir/flags.make

CMakeFiles/huawei_.dir/main.cc.o: CMakeFiles/huawei_.dir/flags.make
CMakeFiles/huawei_.dir/main.cc.o: ../main.cc
CMakeFiles/huawei_.dir/main.cc.o: CMakeFiles/huawei_.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chr_523/Work_all/huawei_partition/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/huawei_.dir/main.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/huawei_.dir/main.cc.o -MF CMakeFiles/huawei_.dir/main.cc.o.d -o CMakeFiles/huawei_.dir/main.cc.o -c /home/chr_523/Work_all/huawei_partition/main/main.cc

CMakeFiles/huawei_.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huawei_.dir/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chr_523/Work_all/huawei_partition/main/main.cc > CMakeFiles/huawei_.dir/main.cc.i

CMakeFiles/huawei_.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huawei_.dir/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chr_523/Work_all/huawei_partition/main/main.cc -o CMakeFiles/huawei_.dir/main.cc.s

# Object files for target huawei_
huawei__OBJECTS = \
"CMakeFiles/huawei_.dir/main.cc.o"

# External object files for target huawei_
huawei__EXTERNAL_OBJECTS =

test_bison: CMakeFiles/huawei_.dir/main.cc.o
test_bison: CMakeFiles/huawei_.dir/build.make
test_bison: paser/libverilogparser.a
test_bison: CMakeFiles/huawei_.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chr_523/Work_all/huawei_partition/main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_bison"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/huawei_.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/huawei_.dir/build: test_bison
.PHONY : CMakeFiles/huawei_.dir/build

CMakeFiles/huawei_.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/huawei_.dir/cmake_clean.cmake
.PHONY : CMakeFiles/huawei_.dir/clean

CMakeFiles/huawei_.dir/depend:
	cd /home/chr_523/Work_all/huawei_partition/main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chr_523/Work_all/huawei_partition/main /home/chr_523/Work_all/huawei_partition/main /home/chr_523/Work_all/huawei_partition/main/build /home/chr_523/Work_all/huawei_partition/main/build /home/chr_523/Work_all/huawei_partition/main/build/CMakeFiles/huawei_.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/huawei_.dir/depend

