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
CMAKE_COMMAND = /snap/clion/137/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/137/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniela/CLionProjects/serverhttp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniela/CLionProjects/serverhttp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/serverhttp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/serverhttp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/serverhttp.dir/flags.make

CMakeFiles/serverhttp.dir/main.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/serverhttp.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/main.cpp.o -c /home/daniela/CLionProjects/serverhttp/main.cpp

CMakeFiles/serverhttp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/main.cpp > CMakeFiles/serverhttp.dir/main.cpp.i

CMakeFiles/serverhttp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/main.cpp -o CMakeFiles/serverhttp.dir/main.cpp.s

CMakeFiles/serverhttp.dir/crypto.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/crypto.cpp.o: ../crypto.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/serverhttp.dir/crypto.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/crypto.cpp.o -c /home/daniela/CLionProjects/serverhttp/crypto.cpp

CMakeFiles/serverhttp.dir/crypto.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/crypto.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/crypto.cpp > CMakeFiles/serverhttp.dir/crypto.cpp.i

CMakeFiles/serverhttp.dir/crypto.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/crypto.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/crypto.cpp -o CMakeFiles/serverhttp.dir/crypto.cpp.s

CMakeFiles/serverhttp.dir/connection.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/connection.cpp.o: ../connection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/serverhttp.dir/connection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/connection.cpp.o -c /home/daniela/CLionProjects/serverhttp/connection.cpp

CMakeFiles/serverhttp.dir/connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/connection.cpp > CMakeFiles/serverhttp.dir/connection.cpp.i

CMakeFiles/serverhttp.dir/connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/connection.cpp -o CMakeFiles/serverhttp.dir/connection.cpp.s

CMakeFiles/serverhttp.dir/mime_types.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/mime_types.cpp.o: ../mime_types.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/serverhttp.dir/mime_types.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/mime_types.cpp.o -c /home/daniela/CLionProjects/serverhttp/mime_types.cpp

CMakeFiles/serverhttp.dir/mime_types.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/mime_types.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/mime_types.cpp > CMakeFiles/serverhttp.dir/mime_types.cpp.i

CMakeFiles/serverhttp.dir/mime_types.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/mime_types.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/mime_types.cpp -o CMakeFiles/serverhttp.dir/mime_types.cpp.s

CMakeFiles/serverhttp.dir/reply.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/reply.cpp.o: ../reply.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/serverhttp.dir/reply.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/reply.cpp.o -c /home/daniela/CLionProjects/serverhttp/reply.cpp

CMakeFiles/serverhttp.dir/reply.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/reply.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/reply.cpp > CMakeFiles/serverhttp.dir/reply.cpp.i

CMakeFiles/serverhttp.dir/reply.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/reply.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/reply.cpp -o CMakeFiles/serverhttp.dir/reply.cpp.s

CMakeFiles/serverhttp.dir/request_handler.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/request_handler.cpp.o: ../request_handler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/serverhttp.dir/request_handler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/request_handler.cpp.o -c /home/daniela/CLionProjects/serverhttp/request_handler.cpp

CMakeFiles/serverhttp.dir/request_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/request_handler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/request_handler.cpp > CMakeFiles/serverhttp.dir/request_handler.cpp.i

CMakeFiles/serverhttp.dir/request_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/request_handler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/request_handler.cpp -o CMakeFiles/serverhttp.dir/request_handler.cpp.s

CMakeFiles/serverhttp.dir/request_parser.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/request_parser.cpp.o: ../request_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/serverhttp.dir/request_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/request_parser.cpp.o -c /home/daniela/CLionProjects/serverhttp/request_parser.cpp

CMakeFiles/serverhttp.dir/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/request_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/request_parser.cpp > CMakeFiles/serverhttp.dir/request_parser.cpp.i

CMakeFiles/serverhttp.dir/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/request_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/request_parser.cpp -o CMakeFiles/serverhttp.dir/request_parser.cpp.s

CMakeFiles/serverhttp.dir/server.cpp.o: CMakeFiles/serverhttp.dir/flags.make
CMakeFiles/serverhttp.dir/server.cpp.o: ../server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/serverhttp.dir/server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverhttp.dir/server.cpp.o -c /home/daniela/CLionProjects/serverhttp/server.cpp

CMakeFiles/serverhttp.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverhttp.dir/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniela/CLionProjects/serverhttp/server.cpp > CMakeFiles/serverhttp.dir/server.cpp.i

CMakeFiles/serverhttp.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverhttp.dir/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniela/CLionProjects/serverhttp/server.cpp -o CMakeFiles/serverhttp.dir/server.cpp.s

# Object files for target serverhttp
serverhttp_OBJECTS = \
"CMakeFiles/serverhttp.dir/main.cpp.o" \
"CMakeFiles/serverhttp.dir/crypto.cpp.o" \
"CMakeFiles/serverhttp.dir/connection.cpp.o" \
"CMakeFiles/serverhttp.dir/mime_types.cpp.o" \
"CMakeFiles/serverhttp.dir/reply.cpp.o" \
"CMakeFiles/serverhttp.dir/request_handler.cpp.o" \
"CMakeFiles/serverhttp.dir/request_parser.cpp.o" \
"CMakeFiles/serverhttp.dir/server.cpp.o"

# External object files for target serverhttp
serverhttp_EXTERNAL_OBJECTS =

serverhttp: CMakeFiles/serverhttp.dir/main.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/crypto.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/connection.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/mime_types.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/reply.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/request_handler.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/request_parser.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/server.cpp.o
serverhttp: CMakeFiles/serverhttp.dir/build.make
serverhttp: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
serverhttp: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
serverhttp: CMakeFiles/serverhttp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable serverhttp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/serverhttp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/serverhttp.dir/build: serverhttp

.PHONY : CMakeFiles/serverhttp.dir/build

CMakeFiles/serverhttp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/serverhttp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/serverhttp.dir/clean

CMakeFiles/serverhttp.dir/depend:
	cd /home/daniela/CLionProjects/serverhttp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniela/CLionProjects/serverhttp /home/daniela/CLionProjects/serverhttp /home/daniela/CLionProjects/serverhttp/cmake-build-debug /home/daniela/CLionProjects/serverhttp/cmake-build-debug /home/daniela/CLionProjects/serverhttp/cmake-build-debug/CMakeFiles/serverhttp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/serverhttp.dir/depend

