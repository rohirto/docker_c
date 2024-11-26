/**
 * @file log_to_ringbuffer.h
 * @author rohirto
 * @brief Header File for Ring Buffer Logger
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once 

#include <stdio.h>
#include <stdint.h>

typedef struct log_backend_ring_buffer_config
{
    uint8_t *ringBuffer;
    size_t ringBufferLength;
} log_backend_ring_buffer_config_t;

void LOG_InitBackendRingbuffer(log_backend_ring_buffer_config_t *config);
void LOG_DeinitBackendRingbuffer(void);