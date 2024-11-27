/**
 * @file log_config.h
 * @author rohirto
 * @brief Config File for the logger
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#ifndef LOG_ENABLE
#define LOG_ENABLE 1
#endif

#ifndef LOG_ENABLE_COLOR
#define LOG_ENABLE_COLOR 1
#endif

#ifndef LOG_ENABLE_TIMESTAMP
#define LOG_ENABLE_TIMESTAMP 0
#endif

#ifndef LOG_ENABLE_PRINT_LEVEL
#define LOG_ENABLE_PRINT_LEVEL 1
#endif

#ifndef LOG_MAX_MEESSAGE_LENGTH
#define LOG_MAX_MEESSAGE_LENGTH 128
#endif

#define SDK_OS_FREE_RTOS        1
#define LOG_ENABLE_ASYNC_MODE   1

#define LOG_TO_SDK_DEBUG_CONSOLE 0

#define LOG_TO_RING_BUFFER              1
#define LOG_BACKEND_RINGBUFFER_DEBUG    0

#define POSIX_ENV  1
#if !POSIX_ENV
#define NATIVE_ENV  1
#endif
