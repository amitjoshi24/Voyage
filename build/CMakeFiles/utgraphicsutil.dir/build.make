# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/amitjoshi/Desktop/Voyage

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/amitjoshi/Desktop/Voyage/build

# Include any dependencies generated for this target.
include CMakeFiles/utgraphicsutil.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/utgraphicsutil.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/utgraphicsutil.dir/flags.make

CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.o: CMakeFiles/utgraphicsutil.dir/flags.make
CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.o: ../lib/utgraphicsutil/debuggl.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/amitjoshi/Desktop/Voyage/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.o -c /Users/amitjoshi/Desktop/Voyage/lib/utgraphicsutil/debuggl.cc

CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/amitjoshi/Desktop/Voyage/lib/utgraphicsutil/debuggl.cc > CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.i

CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/amitjoshi/Desktop/Voyage/lib/utgraphicsutil/debuggl.cc -o CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.s

CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.o: CMakeFiles/utgraphicsutil.dir/flags.make
CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.o: ../lib/utgraphicsutil/jpegio.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/amitjoshi/Desktop/Voyage/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.o -c /Users/amitjoshi/Desktop/Voyage/lib/utgraphicsutil/jpegio.cc

CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/amitjoshi/Desktop/Voyage/lib/utgraphicsutil/jpegio.cc > CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.i

CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/amitjoshi/Desktop/Voyage/lib/utgraphicsutil/jpegio.cc -o CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.s

# Object files for target utgraphicsutil
utgraphicsutil_OBJECTS = \
"CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.o" \
"CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.o"

# External object files for target utgraphicsutil
utgraphicsutil_EXTERNAL_OBJECTS =

libutgraphicsutil.a: CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/debuggl.cc.o
libutgraphicsutil.a: CMakeFiles/utgraphicsutil.dir/lib/utgraphicsutil/jpegio.cc.o
libutgraphicsutil.a: CMakeFiles/utgraphicsutil.dir/build.make
libutgraphicsutil.a: CMakeFiles/utgraphicsutil.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/amitjoshi/Desktop/Voyage/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libutgraphicsutil.a"
	$(CMAKE_COMMAND) -P CMakeFiles/utgraphicsutil.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utgraphicsutil.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/utgraphicsutil.dir/build: libutgraphicsutil.a

.PHONY : CMakeFiles/utgraphicsutil.dir/build

CMakeFiles/utgraphicsutil.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/utgraphicsutil.dir/cmake_clean.cmake
.PHONY : CMakeFiles/utgraphicsutil.dir/clean

CMakeFiles/utgraphicsutil.dir/depend:
	cd /Users/amitjoshi/Desktop/Voyage/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/amitjoshi/Desktop/Voyage /Users/amitjoshi/Desktop/Voyage /Users/amitjoshi/Desktop/Voyage/build /Users/amitjoshi/Desktop/Voyage/build /Users/amitjoshi/Desktop/Voyage/build/CMakeFiles/utgraphicsutil.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/utgraphicsutil.dir/depend

