# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src

# Include any dependencies generated for this target.
include src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/depend.make

# Include the progress variables for this target.
include src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/progress.make

# Include the compile flags for this target's objects.
include src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/flags.make

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.o: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/flags.make
src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.o: FloatingPoint/fp-math.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.o"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.o -c /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/fp-math.cpp

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.i"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/fp-math.cpp > CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.i

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.s"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/fp-math.cpp -o CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.s

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.o: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/flags.make
src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.o: FloatingPoint/floating-point.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.o"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.o -c /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/floating-point.cpp

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.i"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/floating-point.cpp > CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.i

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.s"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/floating-point.cpp -o CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.s

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.o: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/flags.make
src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.o: FloatingPoint/floating-point-softfloat.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.o"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.o -c /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/floating-point-softfloat.cpp

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.i"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/floating-point-softfloat.cpp > CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.i

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.s"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/floating-point-softfloat.cpp -o CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.s

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.o: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/flags.make
src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.o: FloatingPoint/fixed-point.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.o"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.o -c /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/fixed-point.cpp

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.i"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/fixed-point.cpp > CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.i

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.s"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/fixed-point.cpp -o CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.s

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.o: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/flags.make
src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.o: FloatingPoint/bool-data.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.o"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.o -c /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/bool-data.cpp

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.i"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/bool-data.cpp > CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.i

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.s"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint/bool-data.cpp -o CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.s

# Object files for target SCI-FloatingPoint
SCI__FloatingPoint_OBJECTS = \
"CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.o" \
"CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.o" \
"CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.o" \
"CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.o" \
"CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.o"

# External object files for target SCI-FloatingPoint
SCI__FloatingPoint_EXTERNAL_OBJECTS =

lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fp-math.cpp.o
lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point.cpp.o
lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/floating-point-softfloat.cpp.o
lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/fixed-point.cpp.o
lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/bool-data.cpp.o
lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/build.make
lib/libSCI-FloatingPoint.a: src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library ../../lib/libSCI-FloatingPoint.a"
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && $(CMAKE_COMMAND) -P CMakeFiles/SCI-FloatingPoint.dir/cmake_clean_target.cmake
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SCI-FloatingPoint.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/build: lib/libSCI-FloatingPoint.a

.PHONY : src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/build

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/clean:
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint && $(CMAKE_COMMAND) -P CMakeFiles/SCI-FloatingPoint.dir/cmake_clean.cmake
.PHONY : src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/clean

src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/depend:
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/FloatingPoint /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/src/src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/FloatingPoint/CMakeFiles/SCI-FloatingPoint.dir/depend

