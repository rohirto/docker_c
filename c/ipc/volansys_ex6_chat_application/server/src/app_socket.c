/**
 * @file socket.c
 * @author rohit S
 * @brief 
 * @version 0.1
 * @date 2023-11-09
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include "app_debug.h"

#define MAX_LINE 128

struct fd_state 
{
    char buffer[MAX_LINE];
    size_t buffer_used;

    int writing;
    size_t n_written;
    size_t write_upto;
};

/**
 * @brief Send all bytes in buffer on socket s
 * 
 * @param s Socket file descriptor s 
 * @param buf unsigned char* buffer to be sent
 * @param len int* no of bytes to be sent, updated with actual no of bytes
 * @return int -1 on failure, 0 on success
 */
int sendall(int s, unsigned char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n = 0;
    while (total < *len)
    {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            fprintfRed(stderr,"Error no: %d",errno);
            if (errno == EPIPE || errno == ECONNRESET)
            {
                return -1;
            }
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;            // return number actually sent here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

/**
 * @brief Make the socket Non Blocking, to acheive async functionality
 * 
 * @param fd file descriptor of socket
 * @return int 0 on success, -1 on failure
 */
int make_sock_nonblocking(int fd)
{
    if(fcntl(fd, F_SETFL, O_NONBLOCK) != 0)
    {
        debugError("fcntl");
        return -1;
    }
    return 0;
}

struct fd_state *alloc_fd_state(void)
{
    struct fd_state *state = malloc(sizeof(struct fd_state));
    if (!state)
        return NULL;
    state->buffer_used = state->n_written = state->writing = state->write_upto = 0;
    return state;
}

void
free_fd_state(struct fd_state *state)
{
    free(state);
}

// int do_read(int fd, struct fd_state *state)
// {
//     char buf[1024];
//     int i;
//     ssize_t result;
//     while (1) {
//         result = recv(fd, buf, sizeof(buf), 0);
//         if (result <= 0)
//             break;

//         for (i=0; i < result; ++i)  {
//             if (state->buffer_used < sizeof(state->buffer))
//                 state->buffer[state->buffer_used++] = rot13_char(buf[i]);
//             if (buf[i] == '\n') {
//                 state->writing = 1;
//                 state->write_upto = state->buffer_used;
//             }
//         }
//     }

//     if (result == 0) {
//         return 1;
//     } else if (result < 0) {
//         if (errno == EAGAIN)
//             return 0;
//         return -1;
//     }

//     return 0;
// }
