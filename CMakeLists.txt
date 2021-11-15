cmake_minimum_required(VERSION 3.20)
project(Chip8 CXX)

set(CMAKE_CXX_STANDARD 17)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtkmm-3.0)
include_directories(${GTK_INCLUDE_DIRS})


set(SrcFiles src/Main.cpp src/Memory.cpp src/CodeLoader.cpp)
set(Headers include/CodeLoader.h include/Memory.h include/RegisterFile.h include/Display.h include/Processor.h include/Keyboard.h)

add_executable(Chip8 ${SrcFiles} ${Headers})
set(CMAKE_CXX_FLAGS -pthread)
set_target_properties(Chip8 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Chip8 PRIVATE ${GTK_LIBRARIES})

# Add other flags to the compiler
#target_compile_definitions(Chip8 PRIVATE ${GTK_CFLAGS_OTHER})

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
target_link_directories(Chip8 PRIVATE ${GTK_LIBRARY_DIRS})
