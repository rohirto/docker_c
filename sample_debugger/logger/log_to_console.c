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

#include <stdio.h>
#include "log.h"
#include "log_to_console.h"

static void log_init_backend_debugconsole_puts(uint8_t *buffer, size_t length);

static uint8_t initialized;
LOG_BACKEND_DEFINE(backend_debug_console, log_init_backend_debugconsole_puts, NULL);


static void log_init_backend_debugconsole_puts(uint8_t *buffer, size_t length)
{
    if ((0U == initialized) || (0U == length))
    {
        return;
    }

    fwrite(buffer, 1, length, stdout);

    // for (size_t i = 0; i < length; i++)
    // {
    //     (void)PUTCHAR((int)buffer[i]);
    // }
}

void LOG_InitBackendDebugconsole(void)
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

void LOG_DeinitBackendDebugconsole(void)
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