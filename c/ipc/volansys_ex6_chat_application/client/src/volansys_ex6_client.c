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
#include "app_socket.h"

//defines
#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT "9034"    // Replace with the server's port

#define CONFIG_PACKET       0x01
#define CHAT_INIT_PACKET    0x02
#define MESSAGE_PACKET      0x03


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
    int selected_userID;
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

    int state;

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
            fprintfRed(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return -1;
        }

        // loop through all the results and connect to the first we can
        for (init_cli_context->p = init_cli_context->servinfo; init_cli_context->p != NULL; init_cli_context->p = init_cli_context->p->ai_next)
        {
            if ((init_cli_context->sockfd = socket(init_cli_context->p->ai_family, init_cli_context->p->ai_socktype, // Get descriptor
                                 init_cli_context->p->ai_protocol)) == -1)
            {
                debugError("client: socket");
                continue;
            }
            if (connect(init_cli_context->sockfd, init_cli_context->p->ai_addr, init_cli_context->p->ai_addrlen) == -1) // Connect to the nodes in linked list, ipv4 or ipv6
            {
                close(init_cli_context->sockfd);
                debugError("client: connect");
                continue;
            }
            break;
        }
        if (init_cli_context->p == NULL)
        {
            fprintfRed(stderr, "client: failed to connect\n");
            return -1;
        }

        inet_ntop(init_cli_context->p->ai_family, get_in_addr((struct sockaddr *)init_cli_context->p->ai_addr),
                  init_cli_context->s, sizeof(init_cli_context->s));
        debugLog2("client: connecting to %s\n", init_cli_context->s);
        freeaddrinfo(init_cli_context->servinfo); // all done with this structure

        debugLog2("Connected to the server.\n");

        init_cli_context->state = 0;   //init done
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
    unsigned char input[128];  //8 bytes + 1 null terminator
    while (1) 
    {
        fprintfBlue(stdout, "Enter Username: ");
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
                fprintfRed(stdout,"Username is too long. Please enter up to 8 characters.\n");
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
            fprintfRed(stderr,"Error reading input.\n");
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
        //Send the packet to server
        int len = 10;
        if(sendall(client->sockfd,config_packet,&len) == -1)
        {
            debugError("sendall");
            return -1;
        }

    }

    return 0;


}

int client_hanlde()
{
    client_cnxt* client = get_client_context();
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    fd_set write_fds;
    int fdmax;                          // maximum file descriptor number
    int fd = fileno(stdin);
    int rv;

    //Macros for select()
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);


    //Load the file descriptors into master list
    FD_SET(client->sockfd, &master);  //Socket descriptor
    FD_SET(fd, &master);      //stdin descriptor
    if(client->sockfd > fd)         //Check which one is fdmax
    {
        fdmax = client->sockfd;
    }
    else
    {
        fdmax = fd;
    }
    //User Interaction
    fprintfBlue(stdout,"******* USER NAMES ******************\n");
    fprintfBlue(stdout,"Enter the UserID to Chat with: \n");
    while(1)
    {
        read_fds = master;  //Save a copy of master
        write_fds = master; ////Save a copy of master

        if (select(fdmax + 1, &read_fds, &write_fds, NULL, NULL) == -1)   //we will need both read and write functions
        {
            debugError("select");
            exit(EXIT_FAILURE);
        }
        // run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds)) // If any of the read_fds is set
            {

                if (i == client->sockfd) // If receive on socketfd
                {
                    int status;
                    unsigned char len, packet_type;

                    if ((status = recv(i, &packet_type, 1, 0)) == -1) // first byte of packet is packet type, this is our header
                    {
                        // Error
                        debugError("recv");
                        return -1;
                    }
                    else if (status == 0)
                    {
                        debugLog1_destr("selectserver: socket %d hung up\n", i);
                        close(i);           // bye!
                        FD_CLR(i, &master); // remove from master set
                        return -1;
                    }
                    switch (packet_type)
                    {
                        case CONFIG_PACKET:
                            //List of Username incoming
                            recv(i, &len, 1, 0);
                            unsigned char buffer[128], username[9];
                            int userID;
                            int intPacketLen = (int)len;                 // Cast unsigned char to int
                            if (recvall(i, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
                            {
                                // Error
                                debugError("recv_all");
                                return -1;
                            }
                            unpack(buffer,"8sh", username,&userID);
                            //sscanf(buffer,"%d,%8s",&userID,username);
                            fprintfGreen(stdout,"%d.%s\n",userID, username);
                            client->state = 1;   //Get ready to get userinput about user to select
                            
                            break;
                        case MESSAGE_PACKET:
                            break;
                        default:
                            break;
                    }
                }
                else if(i == fd)  //stdin, every time if something is available on stdin
                {
                    if(client->state == 1) //ready to take input
                    {
                        char buff[5];
                        fgets(buff, sizeof(buff), stdin);
                        buff[strcspn(buff, "\n")] = '\0'; // remove the new line

                        int userID = atoi(buff);  //Got the userID, now need to send to server
                        client->cli_usr.selected_userID = userID;
                        client->state = 2;
                    }
                    if(client->state == 3)  //Take user for message
                    {
                        char buff[128];
                        fgets(buff, sizeof(buff), stdin); //Need a limiter to 128 bytes only
                        buff[strcspn(buff, "\n")] = '\0'; // remove the new line


                        if(strcmp(buff,"q") == 0)// User wants to quit chatting with this user
                        {
                            fprintfRed(stdout,"Stopping Chat...\n");
                            fprintfBlue(stdout,"******* USER NAMES ******************\n");
                            fprintfBlue(stdout,"Enter the UserID to Chat with: \n");
                            
                            client->state = 5;
                        }
                        else
                        {
                            //Send the message
                            strcpy(client->cli_usr.send_msg, buff);
                            client->state = 4;
                        }


                    }
                }
            }
            if (FD_ISSET(i, &write_fds))
            {
                if( i == client->sockfd)
                {
                    if(client->state == 2) //UserID got from user, send to server
                    {
                        unsigned char buff[128];
                        buff[0] = CHAT_INIT_PACKET;
                        int len_to_tx = pack(buff+2,"h",client->cli_usr.selected_userID);
                        buff[1] = len_to_tx;
                        len_to_tx = len_to_tx + 2;
                        if(sendall(client->sockfd,buff,&len_to_tx) == -1)
                        {
                            debugError("send_all");
                            client->state = 0;
                            return -1;
                        }

                        client->state = 3;
                        fprintfBlue(stdout,"Enter your message (q to quit): \n");
                    }
                    if(client->state == 4)
                    {
                        fprintfBlue(stdout,"Sending msg \n");
                        //Send message to Server
                        unsigned char buff[128];
                        buff[0] = MESSAGE_PACKET;
                        int len_to_tx = pack(buff+2,"s",client->cli_usr.send_msg);
                        buff[1] = len_to_tx;
                        len_to_tx = len_to_tx + 2;
                        if(sendall(client->sockfd,buff,&len_to_tx) == -1)
                        {
                            debugError("send_all");
                            client->state = 0;
                            return -1;
                        }
                        client->state = 3;  //Again wait for User to send the message
                    }
                    if(client->state == 5)
                    {
                        if(init_connection() == -1)  //again get the list of active users
                        {
                            debugError("Init Connection failed");
                            return -1;
                        }
                        client->state = 1;
                    }

                }

            }
        }
    }
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
    if(init_connection() == -1)
    {
        debugError("Init Connection failed");
        exit(1);
    }

    //Start the chat server
    debugLog1_constr("%s","Receiving List of Username from Server to Chat\n");
    if(client_hanlde() == -1)
    {
        debugError("client Handle");
        exit(1);
    }




    return 0;
}