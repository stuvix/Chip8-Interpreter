cmake_minimum_required(VERSION 3.20)
project(Chip8)

set(CMAKE_CXX_STANDARD 17)

set(SrcFiles src/Main.cpp src/Memory.cpp src/CodeLoader.cpp)
set(Headers include/CodeLoader.h include/Memory.h include/RegisterFile.h include/Display.h include/Processor.h)

add_executable(Chip8 ${SrcFiles} ${Headers})

set_target_properties(Chip8 PROPERTIES LINKER_LANGUAGE CXX)