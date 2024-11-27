/**
 * @file log_to_ringbuffer.c
 * @author rohirto
 * @brief Ring Buffer Logger
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "log.h"
#include "log_to_ringbuffer.h"
#include "debug_logging.h"

//Structs
typedef struct log_backend_ring_buffer
{
    uint8_t *buffer;
    size_t max;
    size_t head;
    size_t tail;
    bool initialized;
    bool empty;
} log_backend_ring_buffer_t;

//Prototypes
static void log_backend_ringbuffer_push(uint8_t *buffer, size_t length);
static void log_backend_ringbuffer_pop(uint8_t *buffer, size_t length, size_t *outLength);
static size_t ring_buffer_get_freespace(void);

//Global Variables
static log_backend_ring_buffer_t s_logBackendRingBuffer = { 0 };

LOG_BACKEND_DEFINE(backend_ring_buffer, log_backend_ringbuffer_push, log_backend_ringbuffer_pop);


static void print_ringbuffer_state(void)
{
#if LOG_BACKEND_RINGBUFFER_DEBUG
    DLOG_INFO("\n  [RingBuffer]:\n");
    DLOG_INFO("    buffer: %s\n", s_logBackendRingBuffer.buffer);
    DLOG_INFO("    max:    %zu\n", s_logBackendRingBuffer.max);
    DLOG_INFO("    head:   %zu\n", s_logBackendRingBuffer.head);
    DLOG_INFO("    tail:   %zu\n", s_logBackendRingBuffer.tail);
    DLOG_INFO("    empty:  %d\n", s_logBackendRingBuffer.empty);
    DLOG_INFO("    free:   %zu\n", ring_buffer_get_freespace());
#endif
}

static size_t ring_buffer_get_freespace(void)
{
    size_t freeBytes = 0;

    if (s_logBackendRingBuffer.head == s_logBackendRingBuffer.tail)
    {
        if (s_logBackendRingBuffer.empty)
        {
            freeBytes = s_logBackendRingBuffer.max;
        }
        else
        {
            freeBytes = 0;
        }

        goto exit;
    }

    // Tail is before head, so wrapping has occurred.
    if (s_logBackendRingBuffer.head < s_logBackendRingBuffer.tail)
    {
        freeBytes = s_logBackendRingBuffer.tail - s_logBackendRingBuffer.head;
    }
    // Head is above tail, no wrapping.
    else
    {
        freeBytes = s_logBackendRingBuffer.max - (s_logBackendRingBuffer.head - s_logBackendRingBuffer.tail);
    }

exit:
    return freeBytes;
}


static void log_backend_ringbuffer_push(uint8_t *buffer, size_t length)
{
    if (!s_logBackendRingBuffer.initialized)
    {
        return;
    }

    // If the ask is to write more than the buffer supports, chop off the end.
    if (length > s_logBackendRingBuffer.max)
    {
        length = s_logBackendRingBuffer.max;
    }

#if LOG_BACKEND_RINGBUFFER_DEBUG
    DLOG_INFO("%s: Pushing %d bytes\n", __func__, length);
#endif

    // Get free space.
    size_t freeSpace = ring_buffer_get_freespace();

    // Deal with case where buffer is full...

    // Break into two writes as we hit the end of the buffer.
    if (length > s_logBackendRingBuffer.max - s_logBackendRingBuffer.head)
    {
        size_t firstChunk = s_logBackendRingBuffer.max - s_logBackendRingBuffer.head;

        // Fill from current head to the end of the buffer.
        memcpy(&s_logBackendRingBuffer.buffer[s_logBackendRingBuffer.head], buffer, firstChunk);

        // Fill in the rest at the start of the buffer.
        memcpy(&s_logBackendRingBuffer.buffer[0], &buffer[firstChunk], length - firstChunk);

        if (length == firstChunk)
        {
            s_logBackendRingBuffer.head = 0;
        }
        else
        {
            s_logBackendRingBuffer.head = length - firstChunk;
        }

        // Deal with overwrite situation; buffer becomes full.
        if (length > freeSpace)
        {
            s_logBackendRingBuffer.tail = s_logBackendRingBuffer.head;   
        }
    }
    else
    {
        memcpy(&s_logBackendRingBuffer.buffer[s_logBackendRingBuffer.head], buffer, length);

        // If you add the same amount that the buffer can hold, the head does not move.
        if (length != s_logBackendRingBuffer.max)
        {
            s_logBackendRingBuffer.head += length;
        }
    }

    s_logBackendRingBuffer.empty = false;

    print_ringbuffer_state();
}

static void log_backend_ringbuffer_pop(uint8_t *buffer, size_t length, size_t *outLength)
{
    if (!s_logBackendRingBuffer.initialized || s_logBackendRingBuffer.empty)
    {
        return;
    }

    size_t toFetch = 0;
    size_t consumedSpace = s_logBackendRingBuffer.max - ring_buffer_get_freespace();

    // We can only fetch as many bytes as are present in the buffer.
    if (length < consumedSpace)
    {
        toFetch = length;
    }
    else
    {
        toFetch = consumedSpace;
    }

#if LOG_BACKEND_RINGBUFFER_DEBUG
    DLOG_INFO("%s: Popping %d bytes\n", __func__, length);
#endif

    // Break into two reads as we hit the end of the buffer.
    if (toFetch > s_logBackendRingBuffer.max - s_logBackendRingBuffer.tail)
    {
        size_t firstChunk = s_logBackendRingBuffer.max - s_logBackendRingBuffer.tail;

        // Fetch from the current tail to the end of the buffer.
        memcpy(buffer, &s_logBackendRingBuffer.buffer[s_logBackendRingBuffer.tail], firstChunk);

        // Fetch the rest from the start of the buffer.
        memcpy(&buffer[firstChunk], &s_logBackendRingBuffer.buffer[0], toFetch - firstChunk);

        s_logBackendRingBuffer.tail = toFetch - firstChunk;
    }
    else
    {
        memcpy(buffer, &s_logBackendRingBuffer.buffer[s_logBackendRingBuffer.tail], toFetch);

        // If you remove the same amount that the buffer can hold, the tail does not move.
        if (length != s_logBackendRingBuffer.max)
        {
            s_logBackendRingBuffer.tail += toFetch;
        }
    }

    if (toFetch == consumedSpace)
    {
        s_logBackendRingBuffer.empty = true;
    }

    *outLength = toFetch;

    print_ringbuffer_state();
}

void LOG_InitBackendRingbuffer(log_backend_ring_buffer_config_t *config)
{
    logger_status_t ret;
    assert((NULL != config) && (NULL != config->ringBuffer) && (0U != config->ringBufferLength));

    if (s_logBackendRingBuffer.initialized)
    {
        return;
    }

    (void)memset(&s_logBackendRingBuffer, 0, sizeof(s_logBackendRingBuffer));

    s_logBackendRingBuffer.buffer = config->ringBuffer;
    s_logBackendRingBuffer.max = config->ringBufferLength;
    s_logBackendRingBuffer.empty = true;

    ret = LOG_BackendRegister(&backend_ring_buffer);
    if (LOG_Success == ret)
    {
        s_logBackendRingBuffer.initialized = 1U;
    }
    assert(LOG_Success == ret);

    print_ringbuffer_state();

    return;
}

void LOG_DeinitBackendRingbuffer(void)
{
    logger_status_t ret;

    ret = LOG_BackendUnregister(&backend_ring_buffer);
    if (LOG_Success == ret)
    {
        s_logBackendRingBuffer.initialized = 0U;
    }
    
    assert(LOG_Success == ret);

    return;
}