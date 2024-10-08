/**
 * @file circular_buffers.h
 * @author rohirto
 * @brief Header file for Circular Buffer Implementation for SSL read
 * @version 0.1
 * @date 2024-10-08
 * 
 * @copyright Copyright (c) 2024
 * This file is directly included if building using Task Build C program using mbedtls
 * 
 */
#ifndef CIRCULAR_BUFFERS_H
#define CIRCULAR_BUFFERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mbedtls/ssl.h>

#define BUFFER_SIZE 1024  // Define the size of the circular buffer
#define SSL_READ_BUFFER_SIZE 512  // Define the size of SSL read buffer

typedef struct {
    unsigned char buffer[BUFFER_SIZE];
    int head;
    int tail;
    int full;
} CircularBuffer;



//Prototypes
void cb_init(CircularBuffer *cb);
int cb_is_empty(CircularBuffer *cb);
void cb_advance_pointer(CircularBuffer *cb);
void cb_push(CircularBuffer *cb, unsigned char data);
int cb_pop(CircularBuffer *cb, unsigned char *data);
int ssl_read_to_circular_buffer(mbedtls_ssl_context *ssl, CircularBuffer *cb);


#endif