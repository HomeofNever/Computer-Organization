# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/hw5.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw5.dir/flags.make

CMakeFiles/hw5.dir/main.c.o: CMakeFiles/hw5.dir/flags.make
CMakeFiles/hw5.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw5.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw5.dir/main.c.o   -c "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/main.c"

CMakeFiles/hw5.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw5.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/main.c" > CMakeFiles/hw5.dir/main.c.i

CMakeFiles/hw5.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw5.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/main.c" -o CMakeFiles/hw5.dir/main.c.s

# Object files for target hw5
hw5_OBJECTS = \
"CMakeFiles/hw5.dir/main.c.o"

# External object files for target hw5
hw5_EXTERNAL_OBJECTS =

hw5: CMakeFiles/hw5.dir/main.c.o
hw5: CMakeFiles/hw5.dir/build.make
hw5: CMakeFiles/hw5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable hw5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw5.dir/build: hw5

.PHONY : CMakeFiles/hw5.dir/build

CMakeFiles/hw5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw5.dir/clean

CMakeFiles/hw5.dir/depend:
	cd "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5" "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5" "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug" "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug" "/mnt/c/Users/luox6/OneDrive - Rensselaer Polytechnic Institute/Documents/Project/CO/CSCI2500-19FALL/hw/hw5/cmake-build-debug/CMakeFiles/hw5.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/hw5.dir/depend
