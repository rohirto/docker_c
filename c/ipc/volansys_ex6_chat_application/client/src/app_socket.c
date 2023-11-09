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