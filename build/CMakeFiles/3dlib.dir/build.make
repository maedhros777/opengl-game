# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/michael/C++/3DLib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michael/C++/3DLib

# Include any dependencies generated for this target.
include build/CMakeFiles/3dlib.dir/depend.make

# Include the progress variables for this target.
include build/CMakeFiles/3dlib.dir/progress.make

# Include the compile flags for this target's objects.
include build/CMakeFiles/3dlib.dir/flags.make

build/CMakeFiles/3dlib.dir/__/main.cpp.o: build/CMakeFiles/3dlib.dir/flags.make
build/CMakeFiles/3dlib.dir/__/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/michael/C++/3DLib/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object build/CMakeFiles/3dlib.dir/__/main.cpp.o"
	cd /home/michael/C++/3DLib/build && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/3dlib.dir/__/main.cpp.o -c /home/michael/C++/3DLib/main.cpp

build/CMakeFiles/3dlib.dir/__/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/3dlib.dir/__/main.cpp.i"
	cd /home/michael/C++/3DLib/build && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/michael/C++/3DLib/main.cpp > CMakeFiles/3dlib.dir/__/main.cpp.i

build/CMakeFiles/3dlib.dir/__/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/3dlib.dir/__/main.cpp.s"
	cd /home/michael/C++/3DLib/build && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/michael/C++/3DLib/main.cpp -o CMakeFiles/3dlib.dir/__/main.cpp.s

build/CMakeFiles/3dlib.dir/__/main.cpp.o.requires:
.PHONY : build/CMakeFiles/3dlib.dir/__/main.cpp.o.requires

build/CMakeFiles/3dlib.dir/__/main.cpp.o.provides: build/CMakeFiles/3dlib.dir/__/main.cpp.o.requires
	$(MAKE) -f build/CMakeFiles/3dlib.dir/build.make build/CMakeFiles/3dlib.dir/__/main.cpp.o.provides.build
.PHONY : build/CMakeFiles/3dlib.dir/__/main.cpp.o.provides

build/CMakeFiles/3dlib.dir/__/main.cpp.o.provides.build: build/CMakeFiles/3dlib.dir/__/main.cpp.o
.PHONY : build/CMakeFiles/3dlib.dir/__/main.cpp.o.provides.build

# Object files for target 3dlib
3dlib_OBJECTS = \
"CMakeFiles/3dlib.dir/__/main.cpp.o"

# External object files for target 3dlib
3dlib_EXTERNAL_OBJECTS =

build/3dlib: build/CMakeFiles/3dlib.dir/__/main.cpp.o
build/3dlib: /usr/lib/libGLU.so
build/3dlib: /usr/lib/libGL.so
build/3dlib: /usr/lib/libSM.so
build/3dlib: /usr/lib/libICE.so
build/3dlib: /usr/lib/libX11.so
build/3dlib: /usr/lib/libXext.so
build/3dlib: /usr/lib/libSDLmain.a
build/3dlib: /usr/lib/libSDL.so
build/3dlib: /usr/lib/libGLU.so
build/3dlib: /usr/lib/libGL.so
build/3dlib: /usr/lib/libSM.so
build/3dlib: /usr/lib/libICE.so
build/3dlib: /usr/lib/libX11.so
build/3dlib: /usr/lib/libXext.so
build/3dlib: source/libsource.a
build/3dlib: /usr/lib/libSDLmain.a
build/3dlib: /usr/lib/libSDL.so
build/3dlib: build/CMakeFiles/3dlib.dir/build.make
build/3dlib: build/CMakeFiles/3dlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable 3dlib"
	cd /home/michael/C++/3DLib/build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3dlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
build/CMakeFiles/3dlib.dir/build: build/3dlib
.PHONY : build/CMakeFiles/3dlib.dir/build

build/CMakeFiles/3dlib.dir/requires: build/CMakeFiles/3dlib.dir/__/main.cpp.o.requires
.PHONY : build/CMakeFiles/3dlib.dir/requires

build/CMakeFiles/3dlib.dir/clean:
	cd /home/michael/C++/3DLib/build && $(CMAKE_COMMAND) -P CMakeFiles/3dlib.dir/cmake_clean.cmake
.PHONY : build/CMakeFiles/3dlib.dir/clean

build/CMakeFiles/3dlib.dir/depend:
	cd /home/michael/C++/3DLib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michael/C++/3DLib /home/michael/C++/3DLib/build /home/michael/C++/3DLib /home/michael/C++/3DLib/build /home/michael/C++/3DLib/build/CMakeFiles/3dlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : build/CMakeFiles/3dlib.dir/depend

