# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Create the project
project(tlsf_example)


# Add source files for example_1 (hello.c and hello.h)
set(SOURCES
    example/main.c
    thirdparty/tlsf.c
    thirdparty/tlsf.h
)

include_directories(thirdparty)


# Add an executable for example_1
add_executable(tlsf_ex ${SOURCES})

