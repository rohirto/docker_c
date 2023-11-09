/**
 * @file app_socket.h
 * @author rohit s
 * @brief app socket header file which contains various functions related to socket
 * @version 0.1
 * @date 2023-11-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef APP_SOCKET_H
#define APP_SOCKET_H

int sendall(int , unsigned char *, int *);
int make_sock_nonblocking(int fd);

#endif