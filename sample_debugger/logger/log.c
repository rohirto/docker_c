/**
 * @file log.c
 * @author rohirto
 * @brief 
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "log.h"

#ifdef SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"
#endif

//Defines
typedef struct log_print_buffer
{
    uint8_t *buffer;
    size_t length;
    size_t sofar;
} log_print_buffer_t;

typedef struct log_context
{
    log_backend_t *backend;
#if LOG_ENABLE_TIMESTAMP
    log_get_timestamp_callback_t getTimeStamp;
#endif
#ifdef SDK_OS_FREE_RTOS
    SemaphoreHandle_t mutex;
#endif
    uint8_t initialized;
} log_context_t;

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
#define LOG_COLOR_PRINT_PARAMETER(level) log_get_color(level), log_get_level_name(level), log_get_color(LOG_LevelNone)
#else
#define LOG_COLOR_PRINT                  " %s "
#define LOG_COLOR_PRINT_PARAMETER(level) log_get_level_name(level)
#endif


//Variables
static log_context_t s_logContext;
#if LOG_ENABLE_COLOR
static const char *s_logColor[] = {
    LOG_COLOR_CODE_DEFAULT, LOG_COLOR_CODE_MAGENTA, LOG_COLOR_CODE_RED,  LOG_COLOR_CODE_YELLOW,
    LOG_COLOR_CODE_GREEN,   LOG_COLOR_CODE_CYAN,    LOG_COLOR_CODE_BLUE,
};
#endif
static const char *s_logLevelName[] = {
    "None", "FATAL", "ERROR", "WARN ", "INFO ", "DEBUG", "TRACE",
};

//Private funcs
static void log_output_low_level(uint8_t *buffer, size_t length)
{
    log_backend_t *backend = s_logContext.backend;

    while (NULL != backend)
    {
        if (NULL != backend->push)
        {
            backend->push(buffer, length);
        }
        backend = backend->next;
    }
}

static void log_print_to_buffer(char *buf, int32_t *indicator, char val, int len)
{
    log_print_buffer_t *buffer = (log_print_buffer_t *)((void *)(buf));
    int i                      = 0;

    for (i = 0; i < len; i++)
    {
        if ((((size_t)(*indicator)) + buffer->sofar + ((size_t)1)) >= buffer->length)
        {
            break;
        }

        // if ((val == '\n') &&
        //     ((0 == (*indicator)) ||
        //      (((uint8_t)'\r') != buffer->buffer[((size_t)(*indicator)) + buffer->sofar - ((size_t)1)])))
        // {
        //     buffer->buffer[((size_t)(*indicator)) + buffer->sofar] = (uint8_t)'\r';
        //     (*indicator)++;
        // }

        buffer->buffer[((size_t)(*indicator)) + buffer->sofar] = (uint8_t)val;
        (*indicator)++;
    }
}

static int log_sprintf_internal(log_print_buffer_t *printBuf, char const *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    /* format print log first */
    ret = StrFormatPrintf(format, ap, (char *)printBuf, log_print_to_buffer);  //What is this?
    va_end(ap);

    return ret;
}

static char const *log_get_level_name(log_level_t level)
{
    assert(((uint8_t)level < ARRAY_SIZE(s_logLevelName)));
    return s_logLevelName[(int)level];
}

#if LOG_ENABLE_COLOR
static char const *log_get_color(log_level_t level)
{
    assert(((uint8_t)level < ARRAY_SIZE(s_logColor)));
    return s_logColor[(int)level];
}
#endif

log_status_t LOG_Init(void)
{
#if (LOG_ENABLE_ASYNC_MODE > 0)
    uint8_t i;
#endif

    if (0U != s_logContext.initialized)
    {
        return Status_LOG_Initialized;
    }

    (void)memset(&s_logContext, 0, sizeof(s_logContext));

#ifdef SDK_OS_FREE_RTOS
    s_logContext.mutex = xSemaphoreCreateRecursiveMutex();
    if (NULL == s_logContext.mutex)
    {
        return kStatus_LOG_LackResource;
    }
#endif
    s_logContext.initialized = 1U;

    return Status_LOG_Success;
}

log_status_t LOG_Deinit(void)
{
    if (0U == s_logContext.initialized)
    {
        return Status_LOG_Uninitialized;
    }
#ifdef SDK_OS_FREE_RTOS
    if (NULL != s_logContext.mutex)
    {
        vSemaphoreDelete(s_logContext.mutex);
        s_logContext.mutex = NULL;
    }
#endif
    s_logContext.initialized = 0U;
    return Status_LOG_Success;
}

#if LOG_ENABLE_TIMESTAMP
log_status_t LOG_SetTimestamp(log_get_timestamp_callback_t getTimeStamp)
{
    if (0U == s_logContext.initialized)
    {
        return Status_LOG_Uninitialized;
    }

    s_logContext.getTimeStamp = getTimeStamp;

    return Status_LOG_Success;
}

unsigned int LOG_GetTimestamp(void)
{
    if (0U == s_logContext.initialized)
    {
        return 0;
    }

    if (NULL != s_logContext.getTimeStamp)
    {
        return s_logContext.getTimeStamp();
    }
    return 0;
}
#endif

void LOG_Printf(log_module_t const *module, log_level_t level, unsigned int timeStamp, char const *format, ...)
{
    va_list ap;
    log_print_buffer_t buffer;
    uint8_t printBuf[LOG_MAX_MEESSAGE_LENGTH];

    (void)module;

    if (0U == s_logContext.initialized)
    {
        return;
    }

#ifdef SDK_OS_FREE_RTOS
    if (((BaseType_t)0) == (BaseType_t)xSemaphoreTakeRecursive(s_logContext.mutex, portMAX_DELAY))
    {
        return;
    }
#endif

    buffer.buffer = printBuf;
    buffer.length = LOG_MAX_MEESSAGE_LENGTH;
    buffer.sofar  = 0;

    /* print timestamp */
#if LOG_ENABLE_TIMESTAMP
    buffer.sofar += (size_t)log_sprintf_internal(&buffer, "%12d:", timeStamp);
#endif
    /* print level name */
#if LOG_ENABLE_LEVEL_PREFIX
    buffer.sofar += (size_t)log_sprintf_internal(&buffer, LOG_COLOR_PRINT, LOG_COLOR_PRINT_PARAMETER(level));
#endif

    va_start(ap, format);
    /* format print log first */
    buffer.sofar += (size_t)StrFormatPrintf(format, ap, (char *)&buffer, log_print_to_buffer);  //TODO: What is this

    va_end(ap);

    log_output_low_level(buffer.buffer, buffer.sofar);

#ifdef SDK_OS_FREE_RTOS
    if (((BaseType_t)0) == (BaseType_t)xSemaphoreGiveRecursive(s_logContext.mutex))
    {
        return;
    }
#endif
}


log_status_t LOG_BackendRegister(log_backend_t *backend)
{
    log_backend_t *p = s_logContext.backend;
    log_status_t ret = Status_LOG_BackendExist;
    uint32_t regPrimask;

    assert(NULL != backend);

    if (0U == s_logContext.initialized)
    {
        return Status_LOG_Uninitialized;
    }

    regPrimask = DisableGlobalIRQ(); //TODO: what is this?
    while (NULL != p)
    {
        if (p == backend)
        {
            break;
        }
        p = p->next;
    }
    if (NULL == p)
    {
        backend->next        = s_logContext.backend;
        s_logContext.backend = backend;
        ret                  = Status_LOG_Success;
    }
    EnableGlobalIRQ(regPrimask);
    return ret;
}

log_status_t LOG_BackendUnregister(log_backend_t *backend)
{
    log_backend_t *p = s_logContext.backend;
    log_backend_t *q = s_logContext.backend;
    log_status_t ret = Status_LOG_BackendNotFound;
    uint32_t regPrimask;

    assert(NULL != backend);

    if (0U == s_logContext.initialized)
    {
        return Status_LOG_Uninitialized;
    }

    regPrimask = DisableGlobalIRQ(); //TODO: What is this?
    while (NULL != p)
    {
        if (p == backend)
        {
            if (q == p)
            {
                s_logContext.backend = p->next;
            }
            else
            {
                q->next = p->next;
            }
            p->next = NULL;
            ret     = Status_LOG_Success;
        }
        q = p;
        p = p->next;
    }
    EnableGlobalIRQ(regPrimask);

    return ret;
}


void LOG_Dump(uint8_t *buffer, size_t length, size_t *outLength)
{
    log_backend_t *backend = s_logContext.backend;

    if (0U == s_logContext.initialized)
    {
        return;
    }

#ifdef SDK_OS_FREE_RTOS
    if (((BaseType_t)0) == (BaseType_t)xSemaphoreTakeRecursive(s_logContext.mutex, portMAX_DELAY))
    {
        return;
    }
#endif

    while (NULL != backend)
    {
        if (NULL != backend->pop)
        {
            backend->pop(buffer, length, outLength);
        }
        backend = backend->next;
    }

#ifdef SDK_OS_FREE_RTOS
    if (((BaseType_t)0) == (BaseType_t)xSemaphoreGiveRecursive(s_logContext.mutex))
    {
        return;
    }
#endif
}


