/**
 * @file debug_logging.h
 * @author rohirto
 * @brief Header File for debugger at Application Layer
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once 

#include <stdio.h>
#include "log.h"


#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

#define DLOG(...)         \
    printf(__VA_ARGS__)

#define DLOG_INFO(...)    \
    printf(COLOR_BLUE);   \
    printf(__VA_ARGS__);  \
    printf(COLOR_RESET)

#define DLOG_ERROR(...)   \
    printf(COLOR_RED);    \
    printf(__VA_ARGS__);  \
    printf(COLOR_RESET)

#define DLOG_WARN(...)    \
    printf(COLOR_YELLOW); \
    printf(__VA_ARGS__);  \
    printf(COLOR_RESET)

#define DLOG_INFO_BUF(BUF, LEN)                             \
    printf(COLOR_BLUE);                                     \
    for (size_t bufIndex = 0; bufIndex < LEN; bufIndex++)   \
    {                                                       \
        if ((bufIndex % 16) == 0)                           \
            printf("\n    ");                               \
        printf("%02x ", BUF[bufIndex]);                     \
    }                                                       \
    printf(COLOR_RESET);                                    \
    printf("\n")

LOG_MODULE_DEFINE("Sample Debugger Ring Buffer", LevelTrace);
// Macros to be used with DBGLOG, not directly to SDK PRINTF.
#define DBGLOG_FATAL(format, ...)   LOG_Printf(&s_LogModuleLogger, LevelFatal, LOG_TIMESTAMP_GET, format, ##__VA_ARGS__);
#define DBGLOG_ERROR(format, ...)   LOG_Printf(&s_LogModuleLogger, LevelError, LOG_TIMESTAMP_GET, format, ##__VA_ARGS__);
#define DBGLOG_WARN(format, ...)    LOG_Printf(&s_LogModuleLogger, LevelWarning, LOG_TIMESTAMP_GET, format, ##__VA_ARGS__);
#define DBGLOG_INFO(format, ...)    LOG_Printf(&s_LogModuleLogger, LevelInfo, LOG_TIMESTAMP_GET, format, ##__VA_ARGS__);
#define DBGLOG_DEBUG(format, ...)   LOG_Printf(&s_LogModuleLogger, LevelDebug, LOG_TIMESTAMP_GET, format, ##__VA_ARGS__);
#define DBGLOG_TRACE(format, ...)   LOG_Printf(&s_LogModuleLogger, LevelTrace, LOG_TIMESTAMP_GET, format, ##__VA_ARGS__);

void DBGLOG_Init(void);
void DBGLOG_SetMessageLevel(log_level_t level);
