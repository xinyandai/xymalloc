# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\app\CLion\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = D:\app\CLion\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\storage\code\xymalloc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\storage\code\xymalloc\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/xymalloc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/xymalloc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/xymalloc.dir/flags.make

CMakeFiles/xymalloc.dir/main.cc.obj: CMakeFiles/xymalloc.dir/flags.make
CMakeFiles/xymalloc.dir/main.cc.obj: CMakeFiles/xymalloc.dir/includes_CXX.rsp
CMakeFiles/xymalloc.dir/main.cc.obj: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\storage\code\xymalloc\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/xymalloc.dir/main.cc.obj"
	D:\env\MinGW\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\xymalloc.dir\main.cc.obj -c D:\storage\code\xymalloc\main.cc

CMakeFiles/xymalloc.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/xymalloc.dir/main.cc.i"
	D:\env\MinGW\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\storage\code\xymalloc\main.cc > CMakeFiles\xymalloc.dir\main.cc.i

CMakeFiles/xymalloc.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/xymalloc.dir/main.cc.s"
	D:\env\MinGW\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\storage\code\xymalloc\main.cc -o CMakeFiles\xymalloc.dir\main.cc.s

CMakeFiles/xymalloc.dir/src/alloc.c.obj: CMakeFiles/xymalloc.dir/flags.make
CMakeFiles/xymalloc.dir/src/alloc.c.obj: CMakeFiles/xymalloc.dir/includes_C.rsp
CMakeFiles/xymalloc.dir/src/alloc.c.obj: ../src/alloc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\storage\code\xymalloc\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/xymalloc.dir/src/alloc.c.obj"
	D:\env\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\xymalloc.dir\src\alloc.c.obj   -c D:\storage\code\xymalloc\src\alloc.c

CMakeFiles/xymalloc.dir/src/alloc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/xymalloc.dir/src/alloc.c.i"
	D:\env\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\storage\code\xymalloc\src\alloc.c > CMakeFiles\xymalloc.dir\src\alloc.c.i

CMakeFiles/xymalloc.dir/src/alloc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/xymalloc.dir/src/alloc.c.s"
	D:\env\MinGW\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\storage\code\xymalloc\src\alloc.c -o CMakeFiles\xymalloc.dir\src\alloc.c.s

# Object files for target xymalloc
xymalloc_OBJECTS = \
"CMakeFiles/xymalloc.dir/main.cc.obj" \
"CMakeFiles/xymalloc.dir/src/alloc.c.obj"

# External object files for target xymalloc
xymalloc_EXTERNAL_OBJECTS =

xymalloc.exe: CMakeFiles/xymalloc.dir/main.cc.obj
xymalloc.exe: CMakeFiles/xymalloc.dir/src/alloc.c.obj
xymalloc.exe: CMakeFiles/xymalloc.dir/build.make
xymalloc.exe: CMakeFiles/xymalloc.dir/linklibs.rsp
xymalloc.exe: CMakeFiles/xymalloc.dir/objects1.rsp
xymalloc.exe: CMakeFiles/xymalloc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\storage\code\xymalloc\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable xymalloc.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\xymalloc.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/xymalloc.dir/build: xymalloc.exe

.PHONY : CMakeFiles/xymalloc.dir/build

CMakeFiles/xymalloc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\xymalloc.dir\cmake_clean.cmake
.PHONY : CMakeFiles/xymalloc.dir/clean

CMakeFiles/xymalloc.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\storage\code\xymalloc D:\storage\code\xymalloc D:\storage\code\xymalloc\cmake-build-debug D:\storage\code\xymalloc\cmake-build-debug D:\storage\code\xymalloc\cmake-build-debug\CMakeFiles\xymalloc.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/xymalloc.dir/depend
