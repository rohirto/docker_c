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
#define SERVER_IP "127.0.0.1"       /**< Replace with the server's IP address */
#define SERVER_PORT "9034"          /**< Replace with the server's port */

#define CONFIG_PACKET       0x01    /**< Config Packets - for config purpose - username getting*/
#define CHAT_INIT_PACKET    0x02    /**< Chat init packet - from client to server only, to let server know which user Id to chat with */
#define MESSAGE_PACKET      0x03    /**< Message Packet - Carries message intended for the other client thread */
#define ERROR_PACKET        0x04    /**< Error packets - from server to client only, when some error has occured */




/**
 * @brief User context struct
 * 
 */
typedef struct user_cntxt
{
    unsigned char username[8];  /**< Username of client */
    //unsigned char password[8];  //8 byte password
    char send_msg[128];         /**< Send message Buffer*/
    unsigned char rx_msg[128];  /**< Rx message Buffer */
    int selected_userID;        /**< User Id to be chatted with */
    int online_userIDs[20];  /**< Max 20 threads, thus max 20 online people*/
    int i;                  /**< Iterator for online ID stack */
}User_Context;


/**
 * @brief Server context struct
 * 
 */
typedef struct Client_Context
{

    struct addrinfo hints;      /**< Some Server related structs */
    struct addrinfo *servinfo; 
    struct addrinfo *p; 

    struct sockaddr_storage remoteaddr;     /**< Client Connection related Variables*/
    socklen_t addrlen;
    char s[INET6_ADDRSTRLEN];
    int sockfd;

    int state;

    User_Context cli_usr;



}client_cnxt;

/**
 * @brief Get the client context object
 * 
 * @return client_cnxt* 
 * @callergraph
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
 * @callergraph
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
 * @brief initialize OnlineIDS array
 * @callergraph
 * 
 */
void init_onlineIDs()
{
    client_cnxt* client = get_client_context();
    for(int i = 0; i < 20; i++)
    {
        client->cli_usr.online_userIDs[i] = 255; //Invalid userIds
    }
    client->cli_usr.i = 0;

}



/**
 * @brief Init the client, establish a connection with server
 * 
 * @return int
 * @callgraph 
 * @callergraph
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


        init_onlineIDs();  //Iterator for stack initialized
        init_cli_context->state = 0;   //init done
    }
    return 0;
}

/**
 * @brief Get the usrname passwd object
 * 
 * @return int - On success 0, on failure -1
 * @callgraph 
 * @callergraph
 */
int get_usrname_passwd()
{
    client_cnxt* username_passwd_context = get_client_context();
    char input[128];  //8 bytes + 1 null terminator
    fprintfBlue(stdout, "Enter Username: ");
    while (1) 
    {
        
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // remove the new line
        if (isValidInput(input, 8,1) == 1)
        {
            // Valid input
            break;

        }
        else
        {
            fprintfRed(stdout, "Please Enter upto chars and alphabets only\n");
        }
    }

    //Got a valid username
    memcpy(username_passwd_context->cli_usr.username,input,8);
    return 0;
}

/**
 * @brief Send username to the server
 * 
 * @return int 
 * @callgraph 
 * @callergraph
 */
int init_connection()
{
    client_cnxt* client = get_client_context();

    if(client != NULL)
    {
        //Send Username to the Server
        if(send_packet(client->sockfd,CONFIG_PACKET,"s",client->cli_usr.username) == -1)
        {
            debugError("sendall");
            return -1;
        }

        //Expect some Ack


    }

    return 0;
}

/**
 * @brief Update the Online IDs array
 * 
 * @param userID 
 * @return int 0 on success -1 if already 20 users in list
 * @callgraph 
 * @callergraph
 */
int update_onlineIDs(int userID)
{
    client_cnxt* client = get_client_context();
    if(client->cli_usr.i < 20)
    {
        client->cli_usr.online_userIDs[client->cli_usr.i++] = userID;
    }
    else
    {
        return -1;  //Already 20 Users in List
    }
    

    return 0;
}

/**
 * @brief Search a USerID in OnlineIDs array
 * 
 * @param userID to be searched in Online UserID arrays
 * @return int 0 if present and valid, -1 if no
 * @callgraph 
 * @callergraph
 */
int search_onlineIDs(int userID)
{
    client_cnxt* client = get_client_context();
    for(int i = 0; i < 20; i++)
    {
        if(client->cli_usr.online_userIDs[i] == userID)
        {
            //Yes the userID is valid 
            return 0;
        }
    }
    return -1;

}


/**
 * @brief Client Handle to handle the connection with server
 * 
 * @return int Usually does not returns, returns -1 on error
 * @callgraph 
 * @callergraph
 */
int client_handle()
{
    client_cnxt* client = get_client_context();
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    fd_set write_fds;
    int fdmax;                          // maximum file descriptor number
    int fd = fileno(stdin);

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
    print_chat_header();
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
                    unsigned char buffer[140];
                    int intPacketLen = 0;

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
                    //Get the len
                    recv(i, &len, 1, 0);
                    switch (packet_type)
                    { 
                        case CONFIG_PACKET:
                            
                            unsigned char username[9];
                            int userID;
                            intPacketLen = (int)len;                 // Cast unsigned char to int
                            if (recvall(i, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
                            {
                                // Error
                                debugError("recv_all");
                                return -1;
                            }
                            unpack(buffer,"8sh", username,&userID);
                            //sscanf(buffer,"%d,%8s",&userID,username);
                            fprintfGreen(stdout,"%d.%s\n",userID, username);
                            //Update the pool of online userIDs
                            if(update_onlineIDs(userID) == -1)
                            {
                                debugError("UserID Full!");
                            }
                            client->state = 1;   //Get ready to get userinput about user to select
                            
                            break;
                        case MESSAGE_PACKET:
                            char message[140];
                            intPacketLen = (int)len; 
                            if (recvall(i, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
                            {
                                // Error
                                debugError("recv_all");
                                return -1;
                            }
                            unpack(buffer,"128s",message);
                            fprintfGreen(stdout,"%s\n",message);
                            break;

                        case ERROR_PACKET:
                            char error[140];
                            intPacketLen = (int)len; 
                            if (recvall(i, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
                            {
                                // Error
                                debugError("recv_all");
                                return -1;
                            }
                            unpack(buffer,"128s",error);
                            fprintfRed(stdout,"%s\n",error);
                            if(strcmp(error,"User already Online!") == 0)
                            {
                                //Exit in this case
                                debugError("user repeat");
                                return -1;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else if(i == fd)  //stdin, every time if something is available on stdin
                {
                    if(client->state == 1) //ready to take input, userID
                    {

                        char buff[5];
                        fgets(buff, sizeof(buff), stdin);
                        buff[strcspn(buff, "\n")] = '\0'; // remove the new line
                        if (isNumber(buff) == 1)
                        {
                            // Is a no.
                            int userID = atoi(buff); // Got the userID, now need to send to server
                            if (search_onlineIDs(userID) == 0)
                            {
                                // Yes the user is Online
                                client->cli_usr.selected_userID = userID;
                                client->state = 2;
                            }
                            else
                            {
                                fprintfRed(stdout, "Invalid UserID, please enter a valid UserID\n");
                            }
                        }
                        else
                        {
                            if (strcmp(buff, "r") == 0)
                            {
                                // Refresh the List
                                print_chat_header();
                                client->state = 5;
                            }
                            else if(strcmp(buff, "q") == 0)
                            {
                                client->state = 6;
                            }
                            else
                            {
                                fprintfRed(stdout, "Not a Number, please enter a valid UserID\n");
                            }
                        }
                    }
                    if (client->state == 3) // Take user for message
                    {
                        char buff[140];
                        fgets(buff, sizeof(buff), stdin); // Need a limiter to 128 bytes only
                        buff[strcspn(buff, "\n")] = '\0'; // remove the new line
                        if (isValidInput(buff,128,0) == 1)
                        {
                            if (strcmp(buff, "q") == 0) // User wants to quit chatting with this user
                            {
                                fprintfRed(stdout, "Stopping Chat and Exiting Application...\n");
                                // User Interaction
                                //print_chat_header();

                                client->state = 6;
                            }
                            else if (strcmp(buff, "r") == 0)
                            {
                                // Refresh the list
                                print_chat_header();
                                client->state = 5;
                            }
                            else
                            {
                                // Send the message
                                strcpy(client->cli_usr.send_msg, buff);
                                client->state = 4;
                            }
                        }
                        else
                        {
                            fprintfRed(stdout, "Invalid input, please retry");
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
                        fprintfBlue(stdout,"Enter your message (q to quit application, r to refresh list): \n");
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
                        init_onlineIDs();
                        if(init_connection() == -1)  //again get the list of active users
                        {
                            debugError("Init Connection failed");
                            return -1;
                        }
                        client->state = 1;
                    }
                    if(client->state == 6)
                    {
                        //Cleanup the Code and Exit
                        return 0;
                    }

                }

            }
        }
    }
}

/**
 * @brief Main Function
 * 
 * @return int 
 * @callgraph 
 * @callergraph
 */
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
    if(client_handle() == -1)
    {
        debugError("client Handle");
        exit(1);
    }

    debugLog1_destr("Exiting Code\n");


    return 0;
}