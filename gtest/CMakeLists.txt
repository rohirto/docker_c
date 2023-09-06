cmake_minimum_required(VERSION 3.10.2) 
project(MyProject)

add_subdirectory(modules)

add_executable(${PROJECT_NAME} mainapp.c)
target_link_libraries(${PROJECT_NAME} calculations)