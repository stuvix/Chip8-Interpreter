cmake_minimum_required(VERSION 3.20)
project(Chip8 CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SrcFiles src/Main.cpp src/Memory.cpp src/CodeLoader.cpp)
set(Headers include/CodeLoader.h include/Memory.h include/RegisterFile.h include/Display.h include/Processor.h include/Keyboard.h)

add_executable(Chip8 ${SrcFiles} ${Headers})
set(CMAKE_CXX_FLAGS -pthread)
set_target_properties(Chip8 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Chip8 Qt5::Widgets)
