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
CMAKE_SOURCE_DIR = /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build

# Include any dependencies generated for this target.
include CMakeFiles/ActorForward_1batch0.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ActorForward_1batch0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ActorForward_1batch0.dir/flags.make

CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.o: CMakeFiles/ActorForward_1batch0.dir/flags.make
CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.o: ../ActorForward_1batch0.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.o -c /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/ActorForward_1batch0.cpp

CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/ActorForward_1batch0.cpp > CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.i

CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/ActorForward_1batch0.cpp -o CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.s

# Object files for target ActorForward_1batch0
ActorForward_1batch0_OBJECTS = \
"CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.o"

# External object files for target ActorForward_1batch0
ActorForward_1batch0_EXTERNAL_OBJECTS =

ActorForward_1batch0: CMakeFiles/ActorForward_1batch0.dir/ActorForward_1batch0.cpp.o
ActorForward_1batch0: CMakeFiles/ActorForward_1batch0.dir/build.make
ActorForward_1batch0: /usr/local/lib/libSCI-FloatBeacon.a
ActorForward_1batch0: /usr/local/lib/libSCI-FloatingPoint.a
ActorForward_1batch0: /usr/local/lib/libSCI-BuildingBlocks.a
ActorForward_1batch0: /usr/local/lib/libSCI-Math.a
ActorForward_1batch0: /usr/local/lib/libSCI-GC.a
ActorForward_1batch0: /usr/local/lib/libSCI-LinearOT.a
ActorForward_1batch0: /usr/local/lib/libSCI-OT.a
ActorForward_1batch0: /usr/local/lib/libSCI-FloatingPoint.a
ActorForward_1batch0: /usr/local/lib/libSCI-BuildingBlocks.a
ActorForward_1batch0: /usr/local/lib/libSCI-Math.a
ActorForward_1batch0: /usr/local/lib/libSCI-GC.a
ActorForward_1batch0: /usr/local/lib/libSCI-LinearOT.a
ActorForward_1batch0: /usr/local/lib/libSCI-OT.a
ActorForward_1batch0: /usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so
ActorForward_1batch0: /usr/lib/x86_64-linux-gnu/libpthread.so
ActorForward_1batch0: /usr/lib/x86_64-linux-gnu/libssl.so
ActorForward_1batch0: /usr/lib/x86_64-linux-gnu/libcrypto.so
ActorForward_1batch0: /usr/lib/x86_64-linux-gnu/libgmp.so
ActorForward_1batch0: CMakeFiles/ActorForward_1batch0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ActorForward_1batch0"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ActorForward_1batch0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ActorForward_1batch0.dir/build: ActorForward_1batch0

.PHONY : CMakeFiles/ActorForward_1batch0.dir/build

CMakeFiles/ActorForward_1batch0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ActorForward_1batch0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ActorForward_1batch0.dir/clean

CMakeFiles/ActorForward_1batch0.dir/depend:
	cd /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build /mnt/c/Users/t-mukherjeea/Downloads/Git/UAI_rebuttal/EzPC/SCI/vital/build/CMakeFiles/ActorForward_1batch0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ActorForward_1batch0.dir/depend

