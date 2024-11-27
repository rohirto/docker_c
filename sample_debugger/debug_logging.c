/**
 * @file debug_logging.c
 * @author rohirto
 * @brief Define Logger at Application layer
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "log.h"
#include "log_to_console.h"
#include "log_to_ringbuffer.h"
#include "debug_logging.h"


#ifndef RING_BUFFER_SIZE
#define RING_BUFFER_SIZE    0x4000
#endif

LOG_MODULE_DEFINE("Sample Debugger Ring Buffer", LevelTrace);
static uint8_t s_ringBuffer[RING_BUFFER_SIZE] = { 0 };

log_backend_ring_buffer_config_t s_ringBufferConfig =
{
    .ringBuffer = s_ringBuffer,
    .ringBufferLength = sizeof(s_ringBuffer)
};

static void init_log_data(void)
{
    DBGLOG_INFO("Test Hello World\n");
}

void DBGLOG_Init(void)
{
    // Initialize the higher-level logging module.
    LOG_Init();

#if LOG_TO_SDK_DEBUG_CONSOLE
    // Attach the SDK debug console to the logging module.
    LOG_InitBackendDebugconsole();
#endif

#if LOG_TO_RING_BUFFER
    // Attach the ring buffer to the logging module.
    LOG_InitBackendRingbuffer(&s_ringBufferConfig);
#endif

    // Provide mechanism to load ring buffer with data, for debug purposes.
    init_log_data();
}

void DBGLOG_SetMessageLevel(log_level_t level)
{
    s_LogModuleLogger.level = level;
}




