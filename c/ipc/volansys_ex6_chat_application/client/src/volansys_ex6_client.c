/**
 * @file volansys_ex6_client.c
 * @author rohit s
 * @brief Client File of Mega Exercises Chat Application
 * @version 0.1
 * @date 2023-11-09
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 * Continuation of Chat Application - Client Side
 * 
 * Client can send 2 types of packets to Server
 * 1. Config Packets - where it sends username and password to the server
 * 2. Message Packet - where it sends Max 128 bytes message to the server
 * 
 * 1. Config Packets - 
 *  [Config Packet 0x01 - 1 byte][len - 1 byte][Username - 8 bytes][Password - Variable len]
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include "app_debug.h"

//defines
#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT "9034"    // Replace with the server's port

#define CONFIG_PACKET   0x01
#define MESSAGE_PACKET  0x02


//Callbacks 
typedef void (*send_packet_cb)(unsigned char*, int); //Callback to send the packet
/**
 * @brief User context struct
 * 
 */
typedef struct user_cntxt
{
    unsigned char username[8];  //8 Bytes username
    unsigned char password[8];  //8 byte password
    unsigned char send_msg[128];
    unsigned char rx_msg[128];
}User_Context;


/**
 * @brief Server context struct
 * 
 */
typedef struct Client_Context
{

    struct addrinfo hints;      /* Some Server related structs */
    struct addrinfo *servinfo; 
    struct addrinfo *p; 

    struct sockaddr_storage remoteaddr;     /* Client Connection related Variables*/
    socklen_t addrlen;
    char s[INET6_ADDRSTRLEN];
    int sockfd;

    User_Context cli_usr;

    send_packet_cb cli_send;



}client_cnxt;

/**
 * @brief Get the client context object
 * 
 * @return client_cnxt* 
 */
client_cnxt* get_client_context()
{
    static client_cnxt chat_client;
    return &chat_client;
}

/**
 * @brief Get the in addr object
 * 
 * @param sa 
 * @return * void* 
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/**
 * @brief Init the client, establish a connection with server
 * 
 * @return int 
 */
int init_client()
{
    client_cnxt* init_cli_context = get_client_context();
    

    if(init_cli_context != NULL)
    {
        // We are good to go
        int rv;
        // filling up the structs
        memset(&init_cli_context->hints, 0, sizeof(init_cli_context->hints));
        init_cli_context->hints.ai_family = AF_UNSPEC;
        init_cli_context->hints.ai_socktype = SOCK_STREAM;

        if ((rv = getaddrinfo(SERVER_IP, SERVER_PORT, &init_cli_context->hints, &init_cli_context->servinfo)) != 0) // Fill up our structs using getaddrinfo function
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return -1;
        }

        // loop through all the results and connect to the first we can
        for (init_cli_context->p = init_cli_context->servinfo; init_cli_context->p != NULL; init_cli_context->p = init_cli_context->p->ai_next)
        {
            if ((init_cli_context->sockfd = socket(init_cli_context->p->ai_family, init_cli_context->p->ai_socktype, // Get descriptor
                                 init_cli_context->p->ai_protocol)) == -1)
            {
                perror("client: socket");
                continue;
            }
            if (connect(init_cli_context->sockfd, init_cli_context->p->ai_addr, init_cli_context->p->ai_addrlen) == -1) // Connect to the nodes in linked list, ipv4 or ipv6
            {
                close(init_cli_context->sockfd);
                perror("client: connect");
                continue;
            }
            break;
        }
        if (init_cli_context->p == NULL)
        {
            fprintf(stderr, "client: failed to connect\n");
            return -1;
        }

        inet_ntop(init_cli_context->p->ai_family, get_in_addr((struct sockaddr *)init_cli_context->p->ai_addr),
                  init_cli_context->s, sizeof(init_cli_context->s));
        printf("\t[+]client: connecting to %s\n", init_cli_context->s);
        freeaddrinfo(init_cli_context->servinfo); // all done with this structure

        printf("\t[+]Connected to the server.\n");
    }
    return 0;
}

/**
 * @brief Get the usrname passwd object
 * 
 * @return int - On success 0, on failure -1
 */
int get_usrname_passwd()
{
    client_cnxt* username_passwd_context = get_client_context();
    unsigned char input[9];  //8 bytes + 1 null terminator
    while (1) 
    {
        printf("\t[*]Enter Username: ");
        if (fgets((char*)input, sizeof(input), stdin)) 
        {
            size_t len = strlen((char*)input);
            if (len > 0 && input[len - 1] == '\n') 
            {
                // Remove the newline character
                input[len - 1] = '\0';
                len--;
            }

            if (len > 8) 
            {
                printf("\t[-]Username is too long. Please enter up to 8 characters.\n");
            } 
            else if (len < 8) 
            {
                // Pad the string with null bytes
                memset(input + len, 0x00, 8 - len);
                break;
            } 
            else 
            {
                break; // Length is exactly 8, no padding needed
            }
        }
        else
        {
            // Handle input error
            fprintf(stderr,"Error reading input.\n");
            return -1;
        }
    }

    //Got a valid username
    memcpy(username_passwd_context->cli_usr.username,input,8);
    return 0;
}

int init_connection()
{
    client_cnxt* client = get_client_context();
    unsigned char config_packet[128];

    if(client != NULL)
    {
        config_packet[0] = CONFIG_PACKET;
        config_packet[1] = 8;  //Actual len
        memcpy(config_packet+2,client->cli_usr.username,8);
    }

    return 0;


}

int main()
{
    //Client Init
    debugLog1_constr("%s","Client Init..\n");
    if(init_client() == -1)
    {
        debugError("Init Client Failed!");
        exit(1);
    }

    //Get Username and Password
    debugLog1_constr("%s","Getting Username and Password\n");
    if(get_usrname_passwd() == -1)
    {
        debugError("Get username password failed");
        exit(1);
    }

    //Send ther server username and password - here server will validate the client and return a list of active users
    debugLog1_constr("%s","Initiating Connection with Server..\n");



    return 0;
}