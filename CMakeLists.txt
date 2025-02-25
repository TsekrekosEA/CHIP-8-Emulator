cmake_minimum_required(VERSION 3.10.0)
project(CHIP-8_Emulator VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)

set(SOURCE_FILES
    src/cpu.cpp
    src/glad.c
    src/display.cpp
    src/memory.cpp
    src/utils.cpp
    )

add_executable(CHIP-8_Emulator ${SOURCE_FILES})

target_link_libraries(CHIP-8_Emulator glfw OpenGL::GL -lGL)
