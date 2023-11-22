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

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "app_config.h"

/**
 * @brief Server context struct
 * 
 */
typedef struct Server_Context
{
    unsigned int no_of_active_connections;  /* No of Active Connections */

    FILE* username;             /* File Pointer of Username file */
    FILE* password;             /* File Pointer of Password File */
    FILE* status;               /* File Pointer of Status File */

    int listener;               /* Listener for new connetions */
    struct addrinfo hints;      /* Some Server related structs */
    struct addrinfo *ai; 
    struct addrinfo *p; 

    struct sockaddr_storage remoteaddr;     /* Client Connection related Variables*/
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int newfd;

#if USE_SELECT
    fd_set readset;             /* File Descriptor sets */
    fd_set writeset;
    fd_set exset;
    fd_set master;
    int maxfd;
#endif


    #if USE_THREADS
    pthread_t thread_pool[THREAD_POOL_SIZE];
    
    #endif



}server_cnxt;

int sendall(int , unsigned char *, int *);
int recvall(int , void *, int *);
int make_sock_nonblocking(int fd);
server_cnxt* get_server_context();
int check_connection(int );
void soft_error_handle(const char*);
void hard_error_handle(const char* );
int send_packet(int , unsigned char , const char*,...);



#endif