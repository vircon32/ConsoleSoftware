# minimum version of CMake that can parse this file
cmake_minimum_required(VERSION 2.8.12...3.19.1)

# By default, project configuration will be Release
# (must be done before project() statement)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Declare the project
project("chip8tovircon32")

# Set compilation flags for C and C++
if (MINGW OR TARGET_OS STREQUAL "linux")
    set(cxx_flags "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Wno-unused-parameter")
    set(c_flags "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter")
elseif (MSVC)
    set(cxx_flags "${CMAKE_CXX_FLAGS} /W3 /EHsc /MP /GS /wd4267 /wd4244")
    set(c_flags "${CMAKE_C_FLAGS} /W3 /MP /GS /wd4267 /wd4244")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()

# Define final executable
add_executable("chip8tovircon32" "./" "Chip8ToVircon32.cpp")
set_property(TARGET ${C_COMPILER_BINARY_NAME} PROPERTY CXX_STANDARD 11)
