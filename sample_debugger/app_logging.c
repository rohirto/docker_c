/**
 * @file app_logging.c
 * @author rohirto
 * @brief Sample debugger for Embedded Applications
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "app_logging.h"

//defines
#ifndef CIRCULAR_BUFFER_SIZE
#define CIRCULAR_BUFFER_SIZE    0x4000
#endif

LOG_MODULE_DEFINE("Sample Circular Buffer Debugger", LOG_LevelTrace);

//Variables
static uint8_t s_circularBuffer[CIRCULAR_BUFFER_SIZE] = { 0 };

log_backend_circular_buffer_config_t s_circularBufferConfig =
{
    .circularBuffer = s_circularBuffer,
    .circularBufferLength = sizeof(s_circularBuffer)
};


//Private Functions
static void init_log_data(void)
{

}


//Public Functions
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