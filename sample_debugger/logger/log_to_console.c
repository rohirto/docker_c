/**
 * @file log_to_console.c
 * @author rohirto
 * @brief Print the Log Context to Console
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//Includes
#include <stdio.h>
#include "log.h"
#include "log_to_console.h"

//Prototypes
static void log_buffer_to_console(uint8_t *buffer, size_t length);

//Global Variables
static uint8_t initialized;
LOG_BACKEND_DEFINE(backend_debug_console, log_buffer_to_console, NULL);

/**
 * @brief Private Function to put a buffer to console
 * 
 * @param buffer 
 * @param length 
 */
static void log_buffer_to_console(uint8_t *buffer, size_t length)
{
    if ((0U == initialized) || (0U == length))
    {
        return;
    }
    #if POSIX_ENV
    fwrite(buffer, 1, length, stdout);
    #else
    #if NATIVE_ENV
    //Buffer Print Function of whatever 
    #endif
    #endif
}

/**
 * @brief Initilaize the a Log Context to debug Console
 * 
 */
void LOGGER_InitLogConsole(void)
{
    logger_status_t ret;
    if (initialized > 0U)
    {
        return;
    }

    ret = LOG_BackendRegister(&backend_debug_console);
    if (LOG_Success == ret)
    {
        initialized = 1U;
    }
    assert(LOG_Success == ret);
    return;
}

void LOGGER_DeinitLogConsole(void)
{
    logger_status_t ret;

    ret = LOG_BackendUnregister(&backend_debug_console);
    if (LOG_Success == ret)
    {
        initialized = 0U;
    }
    assert(LOG_Success == ret);
    return;
}