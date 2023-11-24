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
#include <stdarg.h>
#include "app_debug.h"
#include "app_socket.h"

/**
 * @brief Send all bytes in buffer on socket s
 * 
 * @param s Socket file descriptor s 
 * @param buf unsigned char* buffer to be sent
 * @param len int* no of bytes to be sent, updated with actual no of bytes
 * @return int -1 on failure, 0 on success
 * @callgraph
 * @callergraph
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
 * @brief receive all the bytes of len to socket 
 * @param int s - socket descriptor
 * @param unsigned char *buf - buffer to be rx
 * @param int* len - len to be rx, also how many bytes were actually rx is updated here
 * @returns -1 on failure or 0 on success
 * 
 * @paragraph - to check if socket is closed on other side, just check the *len, if it is zero then connection was closed
 * @callgraph
 * @callergraph
*/
int recvall(int s, void *buf, int *len)
{
    unsigned char* buff = (unsigned char*) buf;
    int total = 0;        // how many bytes we've received
    int bytesleft = *len; // how many bytes we have left to receive
    int n = 0;
    while (total < *len)
    {
        n = recv(s, buff + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;            // return the number actually received here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

/**
 * @brief Make the socket Non Blocking, to acheive async functionality
 * 
 * @param fd file descriptor of socket
 * @return int 0 on success, -1 on failure
 * @callgraph
 * @callergraph
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

/**
 * @brief Send a User Defined Packet
 * 
 * @param socket where the data is to be sent
 * @param packet_type Config, chat init or message packet
 * @param format the format in which variable args are passed, eg "sh" -> string and signed int
 * @param ...  Variable arguments
 * @return int 0 on success -1 on failure
 * @callgraph
 * @callergraph
 * @paragraph Protocol
 *  |Packet Type 1 Byte| len 1 Byte | Body - Max 128 Bytes |
 *  
 *  Max 130 Bytes packet can be there
 */
int send_packet(int socket, unsigned char packet_type, const char *format, ...)
{
    unsigned char buff[130];
    int len_to_tx = 0, l = 0;
    buff[0] = packet_type;


    va_list args;
    va_start(args, format);

    int h;
    char *s;

    for(; *format != '\0'; format++) 
    {
        switch(*format) 
        {
            case 's':
                s = va_arg(args, char*);
                l = pack(buff+(2+len_to_tx),"s",s);
                len_to_tx += l;
                break;
            
            case 'h': // 16-bit
			    h = va_arg(args, int);
                l = pack(buff+(2+len_to_tx),"h",h);
                len_to_tx +=l;
			    break;
            

            default:
                break;
        }
        
    }

    va_end(args);
    buff[1] = len_to_tx;
    len_to_tx = len_to_tx + 2;
    if (sendall(socket, buff, &len_to_tx) == -1)
    {
        debugError("sendall");
        return -1;
    }
    return 0;
}
