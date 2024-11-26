cmake_minimum_required(VERSION 3.16)

## Project Name ##
project(sample_debugger VERSION 1.0)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)


## FreeRTOS Kernel Config ##
# Include the FreeRTOS kernel and configure it for the ARM M0 port so the examples will compile.
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)
set(FREERTOS_PORT GCC_POSIX CACHE STRING \"\")
set(FREERTOS_HEAP 4)
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
add_subdirectory(${FREERTOS_KERNEL_PATH})

## Source Files ##
file(GLOB SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/logger/*.c
)

## Header Files ##
file(GLOB HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/*.h 
    ${CMAKE_CURRENT_LIST_DIR}/logger/*.h
)

## Main Executable ##
add_executable(sample_logger 
    "${CMAKE_CURRENT_LIST_DIR}/main.c"
    "${CMAKE_CURRENT_LIST_DIR}/debug_logging.c"
    "${CMAKE_CURRENT_LIST_DIR}/debug_logging.h"
    "${CMAKE_CURRENT_LIST_DIR}/FreeRTOSConfig.h"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log_config.h"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log_to_console.c"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log_to_console.h"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log_to_ringbuffer.c"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log_to_ringbuffer.h"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log.c"
    "${CMAKE_CURRENT_LIST_DIR}/logger/log.h"
    )

target_include_directories(sample_logger PRIVATE
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/logger
)

## Link FreeRTOS-Cpp ##
target_link_libraries(sample_logger freertos_kernel)