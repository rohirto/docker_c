/**
 * @file circular_buffer_for_ssl_read.c
 * @author rohirto
 * @brief Circular Buffer Implementation for mbedtls read
 * @version 0.1
 * @date 2024-10-08
 * 
 * @copyright Copyright (c) 2024
 * This file is directly included in the build if building using the task Build C mbedtls program
 *
 */
#include "circular_buffers.h"

// Initialize the circular buffer
void cb_init(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->full = 0;
}

// Check if the buffer is empty
int cb_is_empty(CircularBuffer *cb) {
    return (!cb->full && (cb->head == cb->tail));
}

// Check if the buffer is full
int cb_is_full(CircularBuffer *cb) {
    return cb->full;
}

// Advance the head or tail pointer
void cb_advance_pointer(CircularBuffer *cb) {
    if (cb->full) {
        // Overwriting the oldest data, so move the tail forward
        cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    }
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->full = (cb->head == cb->tail);
}

// Store data in the circular buffer
void cb_push(CircularBuffer *cb, unsigned char data) {
    cb->buffer[cb->head] = data;
    cb_advance_pointer(cb);
}

// Retrieve data from the circular buffer
int cb_pop(CircularBuffer *cb, unsigned char *data) {
    if (cb_is_empty(cb)) {
        return -1;  // Buffer is empty
    }

    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->full = 0;  // Once we read data, the buffer is no longer full
    return 0;
}

int ssl_read_to_circular_buffer(mbedtls_ssl_context *ssl, CircularBuffer *cb) {
    unsigned char buf[SSL_READ_BUFFER_SIZE];
    int ret;

    // Read from the SSL connection
    ret = mbedtls_ssl_read(ssl, buf, sizeof(buf));
    if (ret > 0) {
        // Data read successfully, add it to the circular buffer
        for (int i = 0; i < ret; i++) {
            if (!cb_is_full(cb)) {
                cb_push(cb, buf[i]);
            } else {
                // Buffer is full, handle overflow (e.g., discard data or wait)
                printf("Circular buffer overflow\n");
                return -1;
            }
        }
    } else if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
        // The operation would block, handle this case
        printf("Non-blocking operation, try again later.\n");
    } else {
        // An actual error occurred during the read
        printf("SSL read error: -0x%x\n", -ret);
        return -1;
    }

    return 0;
}