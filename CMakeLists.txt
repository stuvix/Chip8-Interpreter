cmake_minimum_required(VERSION 3.20)
project(Chip8 CXX)

# Windows Xcompile
#set(CMAKE_SYSTEM_NAME Windows)
#set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
# adjust the default behavior of the find commands:
# search headers and libraries in the target environment
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# search programs in the host environment
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

set(SrcFiles src/Main.cpp src/Memory.cpp src/CodeLoader.cpp src/RegisterFile.cpp)
set(Headers include/CodeLoader.h include/Memory.h include/RegisterFile.h include/Display.h include/Processor.h include/Keyboard.h src/RegisterFile.cpp)

add_executable(Chip8 ${SrcFiles} ${Headers})
set(CMAKE_CXX_FLAGS -pthread)
set_target_properties(Chip8 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Chip8 Qt5::Widgets Qt5::Multimedia)
