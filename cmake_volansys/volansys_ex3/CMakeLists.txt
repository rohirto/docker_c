# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Create the project
project(my_project)

# Set the output directory to "out"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Add source files for example_1 (hello.c and hello.h)
set(SOURCES_1
    example_1/hello.c
    example_1/hello.h
)

# Add source files for example_2 (hello.cpp and hello.hpp)
set(SOURCES_2
    example_2/hello.cpp
    example_2/hello.hpp
)

# Add an executable for example_1
add_executable(example_1 ${SOURCES_1})

# Add an executable for example_2
add_executable(example_2 ${SOURCES_2})

# Define a macro
add_definitions(-DHELLO_MACRO="Hello from CMake")

# Set an environment variable
set(HELLO_ENV_VARIABLE "Hello from Environment" CACHE STRING "Hello Environment Variable")

# Link the environment variable to the executable
target_compile_definitions(example_2 PRIVATE HELLO_ENV_VARIABLE="${HELLO_ENV_VARIABLE}")
