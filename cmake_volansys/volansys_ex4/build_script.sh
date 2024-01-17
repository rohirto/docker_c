#!/bin/bash

# Set the path to your CMakeLists.txt file
CMAKE_FILE_PATH="/workspaces/docker_c/cmake_volansys/volansys_ex4"

# Set the path to the directory where you want to build
BUILD_DIR="/workspaces/docker_c/cmake_volansys/volansys_ex4/build"

# Run CMake with the toolchain file
cmake -DCMAKE_TOOLCHAIN_FILE=${CMAKE_FILE_PATH}/cmake/arm_toolchain.cmake -S ${CMAKE_FILE_PATH} -B ${BUILD_DIR}

# Build the project
cmake --build ${BUILD_DIR}

# Optionally, install the binaries to a target location
# cmake --install ${BUILD_DIR} --prefix /path/to/installation
