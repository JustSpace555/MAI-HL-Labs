# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_SOURCE_DIR = /home/justspace/share

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/justspace/share

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/justspace/share/CMakeFiles /home/justspace/share//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/justspace/share/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named social_network

# Build rule for target.
social_network: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 social_network
.PHONY : social_network

# fast build rule for target.
social_network/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/build
.PHONY : social_network/fast

config/config.o: config/config.cpp.o
.PHONY : config/config.o

# target to build an object file
config/config.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/config/config.cpp.o
.PHONY : config/config.cpp.o

config/config.i: config/config.cpp.i
.PHONY : config/config.i

# target to preprocess a source file
config/config.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/config/config.cpp.i
.PHONY : config/config.cpp.i

config/config.s: config/config.cpp.s
.PHONY : config/config.s

# target to generate assembly for a file
config/config.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/config/config.cpp.s
.PHONY : config/config.cpp.s

database/database.o: database/database.cpp.o
.PHONY : database/database.o

# target to build an object file
database/database.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/database/database.cpp.o
.PHONY : database/database.cpp.o

database/database.i: database/database.cpp.i
.PHONY : database/database.i

# target to preprocess a source file
database/database.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/database/database.cpp.i
.PHONY : database/database.cpp.i

database/database.s: database/database.cpp.s
.PHONY : database/database.s

# target to generate assembly for a file
database/database.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/database/database.cpp.s
.PHONY : database/database.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/main.cpp.s
.PHONY : main.cpp.s

models/chat/chat.o: models/chat/chat.cpp.o
.PHONY : models/chat/chat.o

# target to build an object file
models/chat/chat.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/chat/chat.cpp.o
.PHONY : models/chat/chat.cpp.o

models/chat/chat.i: models/chat/chat.cpp.i
.PHONY : models/chat/chat.i

# target to preprocess a source file
models/chat/chat.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/chat/chat.cpp.i
.PHONY : models/chat/chat.cpp.i

models/chat/chat.s: models/chat/chat.cpp.s
.PHONY : models/chat/chat.s

# target to generate assembly for a file
models/chat/chat.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/chat/chat.cpp.s
.PHONY : models/chat/chat.cpp.s

models/message/message.o: models/message/message.cpp.o
.PHONY : models/message/message.o

# target to build an object file
models/message/message.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/message/message.cpp.o
.PHONY : models/message/message.cpp.o

models/message/message.i: models/message/message.cpp.i
.PHONY : models/message/message.i

# target to preprocess a source file
models/message/message.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/message/message.cpp.i
.PHONY : models/message/message.cpp.i

models/message/message.s: models/message/message.cpp.s
.PHONY : models/message/message.s

# target to generate assembly for a file
models/message/message.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/message/message.cpp.s
.PHONY : models/message/message.cpp.s

models/post/post.o: models/post/post.cpp.o
.PHONY : models/post/post.o

# target to build an object file
models/post/post.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/post/post.cpp.o
.PHONY : models/post/post.cpp.o

models/post/post.i: models/post/post.cpp.i
.PHONY : models/post/post.i

# target to preprocess a source file
models/post/post.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/post/post.cpp.i
.PHONY : models/post/post.cpp.i

models/post/post.s: models/post/post.cpp.s
.PHONY : models/post/post.s

# target to generate assembly for a file
models/post/post.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/post/post.cpp.s
.PHONY : models/post/post.cpp.s

models/user/user.o: models/user/user.cpp.o
.PHONY : models/user/user.o

# target to build an object file
models/user/user.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/user/user.cpp.o
.PHONY : models/user/user.cpp.o

models/user/user.i: models/user/user.cpp.i
.PHONY : models/user/user.i

# target to preprocess a source file
models/user/user.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/user/user.cpp.i
.PHONY : models/user/user.cpp.i

models/user/user.s: models/user/user.cpp.s
.PHONY : models/user/user.s

# target to generate assembly for a file
models/user/user.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/social_network.dir/build.make CMakeFiles/social_network.dir/models/user/user.cpp.s
.PHONY : models/user/user.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... social_network"
	@echo "... config/config.o"
	@echo "... config/config.i"
	@echo "... config/config.s"
	@echo "... database/database.o"
	@echo "... database/database.i"
	@echo "... database/database.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... models/chat/chat.o"
	@echo "... models/chat/chat.i"
	@echo "... models/chat/chat.s"
	@echo "... models/message/message.o"
	@echo "... models/message/message.i"
	@echo "... models/message/message.s"
	@echo "... models/post/post.o"
	@echo "... models/post/post.i"
	@echo "... models/post/post.s"
	@echo "... models/user/user.o"
	@echo "... models/user/user.i"
	@echo "... models/user/user.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
