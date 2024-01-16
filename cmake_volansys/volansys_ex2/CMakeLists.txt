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

#add sub directory
add_subdirectory(example)


# Create the executable
add_executable(hello.out ${SOURCES})

# Link Library to our executable
target_link_libraries(hello.out PUBLIC example)

# Specify Library's header file to target
target_include_directories(hello.out PUBLIC
                          "${PROJECT_SOURCE_DIR}/example"
                          )

