# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Create the project
project(hello_project)

# Set the output directory to "out"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)


# Set the source files
set(SOURCES src/hello.c)

# Set the include directories
include_directories(inc)

# Create the executable
add_executable(hello.out ${SOURCES})

