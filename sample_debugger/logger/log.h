/**
 * @file log.h
 * @author rohirto
 * @brief 
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "log_config.h"
#if SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"
#endif


typedef enum log_level
{
    LevelNone = 0, /*!< LOG level none */
    LevelFatal,    /*!< LOG level fatal */
    LevelError,    /*!< LOG level error */
    LevelWarning,  /*!< LOG level warning */
    LevelInfo,     /*!< LOG level info */
    LevelDebug,    /*!< LOG level debug */
    LevelTrace,    /*!< LOG level trace */
} log_level_t;

#if LOG_ENABLE_FILE_WITH_PATH > 0
/*!
 * @brief Source file name definition
 * @details Define LOG_FILE_NAME with \__FILE\__ when config LOG_ENABLE_FILE_WITH_PATH is enabled.
 */
#define LOG_FILE_NAME __FILE__
#else
#define LOG_FILE_NAME_INTERCEPT(s, n)                                                           \
    ((sizeof(s) >= (n)) && (((s)[sizeof(s) - (n)] == '/') || ((s)[sizeof(s) - (n)] == '\\'))) ? \
        ((s) + sizeof(s) - ((n)-1))
#define LOG_FILE_NAME_RECURSIVE(f, s, n) \
    f(s, n) : f(s, n + 1) : f(s, n + 2) : f(s, n + 3) : f(s, n + 4) : f(s, n + 5) : f(s, n + 6) : f(s, n + 7)
#define LOG_FILE_NAME_SET(f, f1, s, n) \
    f(f1, s, n) :                      \
        f(f1, s, n + 8) :              \
        f(f1, s, n + 16) :             \
        f(f1, s, n + 24) : f(f1, s, n + 32) : f(f1, s, n + 40) : f(f1, s, n + 48) : f(f1, s, n + 56)
/*!
 * @brief Source file name definition
 * @details There is a macro \__BASE_FILE\__ could be used to get the current source file name in GCC. While
 * the macro is unsupported by IAR in default, the \__BASE_FILE\__ is same as \__FILE\__ in IAR.
 * To support the macro \__BASE_FILE\__, the extra option --no_path_in_file_macros should be added
 * for IAR. But on Keil, only the source file name cannot be got through the macro \__BASE_FILE\__.
 *
 * So, log component adds a macro LOG_FILE_NAME to get the current source file name during the
 * compilation phase, when config LOG_ENABLE_FILE_WITH_PATH is disabled.
 * There is a limitation, the length of file name should be not less than 2,
 * and the supported MAX length of file name is 66 bytes. Otherwise the original string of \__FILE\__
 * will be linked.
 */
#define LOG_FILE_NAME LOG_FILE_NAME_SET(LOG_FILE_NAME_RECURSIVE, LOG_FILE_NAME_INTERCEPT, __FILE__, 3) : __FILE__
#endif

typedef struct log_module
{
    const char *logModuleName; /*!< Log module name */
    log_level_t level;         /*!< Log level of the module */
} log_module_t;

#if LOG_ENABLE_TIMESTAMP
#define LOG_TIMESTAMP_GET LOG_GetTimestamp()
#else
#define LOG_TIMESTAMP_GET 0
#endif

//func Pointers
typedef void (*log_backend_puts_t)(uint8_t *buffer, size_t length);
typedef void (*log_backend_get_dump_buffer_t)(uint8_t *buffer, size_t length, size_t *outLength);

//Structs
typedef struct log_backend
{
    struct log_backend *next;  /*!< Next log backend pointer */
    log_backend_puts_t push; /*!< Put data function of log backend */
    log_backend_get_dump_buffer_t pop;
} log_backend_t;


#define LOG_BACKEND_DEFINE(name, push, pop) static log_backend_t name = {NULL, push, pop}


#if (LOG_ENABLE > 0)

/*!
 * @brief Filter the log
 *
 * @details This macro is used to filter the log. The macro is used by the
 * macro LOG_FATAL/LOG_ERR/LOG_WRN/LOG_INF/LOG_DBG/LOG_TRACE.
 * Only when the following two conditions are met at the same time,
 * 1. The priority of the log message level is valid.
 * 2. The priority of the log message level is higher than the module log
 * level.@n
 * The macro should not be used by application directly.
 */
#define _LOG_PRINTF(logger, logLevel, format, ...)                            \
    if (((logLevel > LevelNone) && ((logger)->level >= logLevel)))       \
    {                                                                         \
        LOG_Printf(logger, logLevel, LOG_TIMESTAMP_GET, format, __VA_ARGS__); \
    }

/*!
 * @brief Defines the log module
 *
 * @details This macro is used to define the log module for each driver/component/middleware.
 * The macro should be added to source code of each module.
 * This is an example, the code block should be placed at the end of the header file
 * including of the source code.
 * The macro is only valid when LOG_ENABLE is set.
 *
 * @code
 * #define LOG_ENABLE 1
 * #include "fsl_component_log.h"
 * LOG_MODULE_DEFINE(hello_world, kLOG_LevelDebug);
 * @endcode
 *
 * @param name The name string of the log module.
 * @param level The debug level of the log module.
 */
#define LOG_MODULE_DEFINE(name, level) static log_module_t s_LogModuleLogger = {#name, level};

/*!
 * @brief Writes the fatal level log formatted output to the backend.
 *
 * @details Call this function to write the fatal level log formatted output to the backend.
 * The log string color feature is set by the macro LOG_ENABLE_COLOR.
 * The log string time stamp feature is set by the macro LOG_ENABLE_TIMESTAMP.
 * The macro is only valid when LOG_ENABLE is set.
 * This is an example,
 * The source code is,
 * @code
 *   LOG_ERR("cycle %d", count);
 * @endcode
 * The output string is,
 * @code
 *            0: FATAL hello_world.c:50:cycle 0
 * @endcode
 *
 * @param   format Format control string.
 */
#define LOG_FATAL(format, ...) \
    _LOG_PRINTF(&s_LogModuleLogger, LevelFatal, "%s:%d:" format "\r\n", LOG_FILE_NAME, __LINE__, ##__VA_ARGS__);

/*!
 * @brief Writes the error level log formatted output to the backend.
 *
 * @details Call this function to write the error level log formatted output to the backend.
 * The log string color feature is set by the macro LOG_ENABLE_COLOR.
 * The log string time stamp feature is set by the macro LOG_ENABLE_TIMESTAMP.
 * The macro is only valid when LOG_ENABLE is set.
 * This is an example,
 * The source code is,
 * @code
 *   LOG_ERR("cycle %d", count);
 * @endcode
 * The output string is,
 * @code
 *            0: ERROR hello_world.c:50:cycle 0
 * @endcode
 *
 * @param   format Format control string.
 */
#define LOG_ERR(format, ...) \
    _LOG_PRINTF(&s_LogModuleLogger, LevelError, "%s:%d:" format "\r\n", LOG_FILE_NAME, __LINE__, ##__VA_ARGS__);

/*!
 * @brief Writes the warning level log formatted output to the backend.
 *
 * @details Call this function to write the warning level log formatted output to the backend.
 * The log string color feature is set by the macro LOG_ENABLE_COLOR.
 * The log string time stamp feature is set by the macro LOG_ENABLE_TIMESTAMP.
 * The macro is only valid when LOG_ENABLE is set.
 * This is an example,
 * The source code is,
 * @code
 *   LOG_WRN("cycle %d", count);
 * @endcode
 * The output string is,
 * @code
 *            0: WARN  hello_world.c:50:cycle 0
 * @endcode
 *
 * @param   format Format control string.
 */
#define LOG_WRN(format, ...) \
    _LOG_PRINTF(&s_LogModuleLogger, LevelWarning, "%s:%d:" format "\r\n", LOG_FILE_NAME, __LINE__, ##__VA_ARGS__);

/*!
 * @brief Writes the info level log formatted output to the backend.
 *
 * @details Call this function to write the info level log formatted output to the backend.
 * The log string color feature is set by the macro LOG_ENABLE_COLOR.
 * The log string time stamp feature is set by the macro LOG_ENABLE_TIMESTAMP.
 * The macro is only valid when LOG_ENABLE is set.
 * This is an example,
 * The source code is,
 * @code
 *   LOG_INF("cycle %d", count);
 * @endcode
 * The output string is,
 * @code
 *            0: INFO  hello_world.c:50:cycle 0
 * @endcode
 *
 * @param   format Format control string.
 */
#define LOG_INF(format, ...) \
    _LOG_PRINTF(&s_LogModuleLogger, LevelInfo, "%s:%d:" format "\r\n", LOG_FILE_NAME, __LINE__, ##__VA_ARGS__);

/*!
 * @brief Writes the debug level log formatted output to the backend.
 *
 * @details Call this function to write the debug level log formatted output to the backend.
 * The log string color feature is set by the macro LOG_ENABLE_COLOR.
 * The log string time stamp feature is set by the macro LOG_ENABLE_TIMESTAMP.
 * The macro is only valid when LOG_ENABLE is set.
 * This is an example,
 * The source code is,
 * @code
 *   LOG_DBG("cycle %d", count);
 * @endcode
 * The output string is,
 * @code
 *            0: DEBUG hello_world.c:50:cycle 0
 * @endcode
 *
 * @param   format Format control string.
 */
#define LOG_DBG(format, ...) \
    _LOG_PRINTF(&s_LogModuleLogger, LevelDebug, "%s:%d:" format "\r\n", LOG_FILE_NAME, __LINE__, ##__VA_ARGS__);

/*!
 * @brief Writes the trace level log formatted output to the backend.
 *
 * @details Call this function to write the trace level log formatted output to the backend.
 * The log string color feature is set by the macro LOG_ENABLE_COLOR.
 * The log string time stamp feature is set by the macro LOG_ENABLE_TIMESTAMP.
 * The macro is only valid when LOG_ENABLE is set.
 * This is an example,
 * The source code is,
 * @code
 *   LOG_TRACE("cycle %d", count);
 * @endcode
 * The output string is,
 * @code
 *            0: TRACE hello_world.c:50:cycle 0
 * @endcode
 *
 * @param   format Format control string.
 */
#define LOG_TRACE(format, ...) \
    _LOG_PRINTF(&s_LogModuleLogger, LevelTrace, "%s:%d:" format "\r\n", LOG_FILE_NAME, __LINE__, ##__VA_ARGS__);

#else

#define LOG_MODULE_DEFINE(name, level)
#define LOG_FATAL(format, ...)
#define LOG_ERR(format, ...)
#define LOG_WRN(format, ...)
#define LOG_INF(format, ...)
#define LOG_DBG(format, ...)
#define LOG_TRACE(format, ...)

#endif



//Enums
typedef enum _logger_status
{
    LOG_Success         = 0,                  /*!< Success */
    LOG_Error           , /*!< Failed */
    LOG_Initialized     , /*!< Initialized */
    LOG_Uninitialized   , /*!< Uninitialized */
    LOG_LackResource    , /*!< Lack resource */
    LOG_BackendExist    , /*!< Backend exists */
    LOG_BackendNotFound , /*!< Backend not found */
} logger_status_t;

#if LOG_ENABLE_COLOR
#define LOG_COLOR_CODE_DEFAULT \
    "\x1B"                     \
    "[0m"
#define LOG_COLOR_CODE_MAGENTA \
    "\x1B"                     \
    "[1;35m"
#define LOG_COLOR_CODE_MAGENTA_BG \
    "\x1B"                        \
    "[0;35m"
#define LOG_COLOR_CODE_RED \
    "\x1B"                 \
    "[1;31m"
#define LOG_COLOR_CODE_RED_BG \
    "\x1B"                    \
    "[0;31m"
#define LOG_COLOR_CODE_YELLOW \
    "\x1B"                    \
    "[1;33m"
#define LOG_COLOR_CODE_YELLOW_BG \
    "\x1B"                       \
    "[0;33m"
#define LOG_COLOR_CODE_GREEN \
    "\x1B"                   \
    "[1;32m"
#define LOG_COLOR_CODE_GREEN_BG \
    "\x1B"                      \
    "[0;32m"
#define LOG_COLOR_CODE_CYAN \
    "\x1b"                  \
    "[1;36m"
#define LOG_COLOR_CODE_CYAN_BG \
    "\x1b"                     \
    "[0;36m"
#define LOG_COLOR_CODE_BLUE \
    "\x1B"                  \
    "[1;34m"
#define LOG_COLOR_CODE_BLUE_BG \
    "\x1B"                     \
    "[0;34m"

#define LOG_COLOR_PRINT                  " %s%s%s "
#define LOG_COLOR_PRINT_PARAMETER(level) log_get_color(level), log_get_level_name(level), log_get_color(kLOG_LevelNone)
#else
#define LOG_COLOR_PRINT                  " %s "
#define LOG_COLOR_PRINT_PARAMETER(level) log_get_level_name(level)
#endif


#if LOG_ENABLE_TIMESTAMP
/*! @brief get time stamp function */
typedef unsigned int (*log_get_timestamp_callback_t)(void);
#endif

//APIs
logger_status_t LOG_Init(void);
logger_status_t LOG_Deinit(void);
void LOG_Printf(log_module_t const *module, log_level_t level, unsigned int timeStamp, char const *format, ...);
void LOG_Dump(uint8_t *buffer, size_t length, size_t *outLength);
logger_status_t LOG_BackendRegister(log_backend_t *backend);
logger_status_t LOG_BackendUnregister(log_backend_t *backend);
#if LOG_ENABLE_TIMESTAMP
logger_status_t LOG_SetTimestamp(log_get_timestamp_callback_t getTimeStamp);
unsigned int LOG_GetTimestamp(void);
#endif

