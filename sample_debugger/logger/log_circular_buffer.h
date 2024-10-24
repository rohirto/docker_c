/**
 * @file log_circular_buffer.h
 * @author rohirto
 * @brief Circular Buffer  Backend Header File
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LOG_CIRCULAR_BUFFER_H
#define LOG_CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdio.h>

//Structs
typedef struct log_backend_circular_buffer_config
{
    uint8_t *circularBuffer;
    size_t circularBufferLength;
} log_backend_circular_buffer_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

void LOG_InitBackendCircularbuffer(log_backend_circular_buffer_config_t *config);
void LOG_DeinitBackendCircularbuffer(void);

#if defined(__cplusplus)
}
#endif

#endif