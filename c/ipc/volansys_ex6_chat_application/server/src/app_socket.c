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
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include "app_debug.h"
#include "app_socket.h"
#include "app_config.h"

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
 * @brief receive all the bytes of len to socket 
 * @param int s - socket descriptor
 * @param unsigned char *buf - buffer to be rx
 * @param int* len - len to be rx, also how many bytes were actually rx is updated here
 * @returns -1 on failure or 0 on success
 * 
 * @paragraph - to check if socket is closed on other side, just check the *len, if it is zero then connection was closed
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
 * @brief Get the server context object
 * 
 * @return server_cnxt* 
 */
server_cnxt* get_server_context()
{
    static server_cnxt chat_server;
    return &chat_server;
}

/**
 * @brief check if the client is still connected or not
 * @param int socket - file descriptor of the client socket 
 * @returns -1 on closed socket, 1 on active socket
*/
int check_connection(int socket)
{
    char data;
    if(recv(socket,&data,1, MSG_PEEK) == 0) //read one byte
    {
        // Socket closed
        perror("recvall");
        fprintf(stderr,"selectserver: socket %d hung up\n", socket);
        return -1;
    }
    return 1;
}

/**
 * @brief Soft error handling on sockets, no exit, just print errorno
 * 
 * @param message Message to be printed
 */
void soft_error_handle(const char* message)
{
    debugError(message);
    fprintfRed(stderr,"Error Code: %d\n",errno);

    //Based on error no return some value
}

/**
 * @brief Hard error handling on sockets, exit with EXIT failure mentioned
 * 
 * @param message Message to be printed on exit
 */
void hard_error_handle(const char* message)
{
    debugError(message);
    fprintfRed(stderr,"Error Code: %d\n",errno);
    exit(EXIT_FAILURE);
}

/**
 * @brief Send a User Defined Packet
 * 
 * @param socket where the data is to be sent
 * @param packet_type Config, chat init or message packet
 * @param format the format in which variable args are passed, eg "sh" -> string and signed int
 * @param ...  Variable arguments
 * @return int 0 on success -1 on failure
 * 
 *  Packet Protocol
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
