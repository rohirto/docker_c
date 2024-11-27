/**
 * @file log.c
 * @author rohirto
 * @brief Log module
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "log.h"
#include <assert.h>
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


// Define the callback type
typedef void (*PrintfCallback)(char *buf, int32_t *indicator, char val, int len);

//Global Variables
static log_context_t s_logContext;


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

// Custom printf-like function
size_t str_format_printf(const char *fmt, va_list ap, char *buf, PrintfCallback cb) 
{
    const char *p = fmt;
    char c;
    int count = 0;
    char temp_buf[64]; // Temporary buffer for small formatted strings

    // Loop through each character in the format string
    while ((c = *p++) != '\0') {
        if (c != '%') {
            // Not a format specifier, directly output the character
            cb(buf, &count, c, 1);
            continue;
        }

        // Handle format specifiers
        c = *p++;
        if (c == '\0') break; // Unexpected end of format string

        switch (c) {
            case 'd': { // Integer
                int value = va_arg(ap, int);
                int len = snprintf(temp_buf, sizeof(temp_buf), "%d", value);
                cb(buf, &count, temp_buf[0], len);
                break;
            }
            case 's': { // String
                const char *value = va_arg(ap, const char *);
                if (value) {
                    size_t len = strlen(value);
                    for (size_t i = 0; i < len; i++) {
                        cb(buf, &count, value[i], 1);
                    }
                }
                break;
            }
            case 'c': { // Character
                char value = (char)va_arg(ap, int);
                cb(buf, &count, value, 1);
                break;
            }
            case '%': { // Escape sequence for '%'
                cb(buf, &count, '%', 1);
                break;
            }
            default:
                // Unsupported format specifier
                cb(buf, &count, '?', 1);
                break;
        }
    }

    return count; // Return the number of characters printed
}


static int log_sprintf_internal(log_print_buffer_t *printBuf, char const *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    /* format print log first */
    ret = str_format_printf(format, ap, (char *)printBuf, log_print_to_buffer);
    va_end(ap);

    return ret;
}

#if LOG_ENABLE_TIMESTAMP
logger_status_t LOG_SetTimestamp(log_get_timestamp_callback_t getTimeStamp)
{
    if (0U == s_logContext.initialized)
    {
        return LOG_Uninitialized;
    }

    s_logContext.getTimeStamp = getTimeStamp;

    return LOG_Success;
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


logger_status_t LOG_Init(void)
{
#if (LOG_ENABLE_ASYNC_MODE > 0)
    uint8_t i;
#endif

    if (0U != s_logContext.initialized)
    {
        return LOG_Initialized;
    }

    (void)memset(&s_logContext, 0, sizeof(s_logContext));

#ifdef SDK_OS_FREE_RTOS
    s_logContext.mutex = xSemaphoreCreateRecursiveMutex();
    if (NULL == s_logContext.mutex)
    {
        return LOG_LackResource;
    }
#endif
    s_logContext.initialized = 1U;

    return LOG_Success;
}



logger_status_t LOG_Deinit(void)
{
    if (0U == s_logContext.initialized)
    {
        return LOG_Uninitialized;
    }
#ifdef SDK_OS_FREE_RTOS
    if (NULL != s_logContext.mutex)
    {
        vSemaphoreDelete(s_logContext.mutex);
        s_logContext.mutex = NULL;
    }
#endif
    s_logContext.initialized = 0U;
    return LOG_Success;
}

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
    buffer.sofar += (size_t)str_format_printf(format, ap, (char *)&buffer, log_print_to_buffer);

    va_end(ap);

    log_output_low_level(buffer.buffer, buffer.sofar);

#ifdef SDK_OS_FREE_RTOS
    if (((BaseType_t)0) == (BaseType_t)xSemaphoreGiveRecursive(s_logContext.mutex))
    {
        return;
    }
#endif
}

logger_status_t LOG_BackendRegister(log_backend_t *backend)
{
    log_backend_t *p = s_logContext.backend;
    logger_status_t ret = LOG_BackendExist;
    uint32_t regPrimask;

    assert(NULL != backend);

    if (0U == s_logContext.initialized)
    {
        return LOG_Uninitialized;
    }

    //regPrimask = DisableGlobalIRQ();
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
        ret                  = LOG_Success;
    }
    //EnableGlobalIRQ(regPrimask);
    return ret;
}



logger_status_t LOG_BackendUnregister(log_backend_t *backend)
{
    log_backend_t *p = s_logContext.backend;
    log_backend_t *q = s_logContext.backend;
    logger_status_t ret = LOG_BackendNotFound;
    //uint32_t regPrimask;

    assert(NULL != backend);

    if (0U == s_logContext.initialized)
    {
        return LOG_Uninitialized;
    }

    //regPrimask = DisableGlobalIRQ();
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
            ret     = LOG_Success;
        }
        q = p;
        p = p->next;
    }
    //EnableGlobalIRQ(regPrimask);

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


