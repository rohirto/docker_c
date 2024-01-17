# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(ARMExample)

# Include the toolchain file
include(cmake/arm_toolchain.cmake)

# Add your source files
add_executable(main main.c)
