/**
 * @file app_event.c
 * @author rohit s
 * @brief Event dispatcher for our client connection event
 * @version 0.1
 * @date 2023-11-12
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include "app_debug.h"
#include "app_socket.h"
#include "app_event.h"
#include <event2/event.h>  //Used for asynchronous sync
#include "app_db.h"
#include "app_queue.h"




/**
 * @brief Dispatch Event function based on the event occured
 * 
 * @param client 
 * @param eventType Read, Write or exception
 * @param eventHandler Handlers are called based on event, implemented using function pointers (callback functions)
 * @return int 0 on success, -1 on soft error, -2 on hard error (client connection closed, release thread for new connection)
 */
int dispatchEvent(User_Context* client, EventType eventType, EventHandler* eventHandler) 
{
    switch (eventType)
    {
    case READ_EVENT:
        if (eventHandler->onRead != NULL)
        {
            unsigned char c;
            int bytesRead;
            if ((bytesRead = recv(client->socket, &c, 1, MSG_PEEK)) == -1)  //Just Peek the message to do some error handling
            {
                soft_error_handle("Socket error\n");
                return -1;
            }
            else if (bytesRead == 0)
            {
                // set the username to offline
                client->status = OFFLINE;
                if (status_handling(client->userID, client->status) == -1)
                {
                    debugError("status update\n");
                }
                debugLog1_destr("Closed Connection on %d\n", client->socket);
                return -2;
            }
            else
            {
                eventHandler->onRead(client);
            }
        }
        break;
    case WRITE_EVENT:
        if (eventHandler->onWrite != NULL)
        {
            eventHandler->onWrite(client);
        }
        break;
    case EXCEPTION_EVENT:
        if (eventHandler->onException != NULL)
        {
            eventHandler->onException(client);
        }
        break;
    default:
        // Handle unknown event type
        break;
    }

    return 0;
}

/**
 * @brief Read Handler, handles incoming data on socket
 * 
 * @param client User context of thread
 */
void onReadHandler(User_Context* client) 
{
    unsigned char packet_type;
    unsigned char len;
    unsigned char buffer[128];
    int intPacketLen = 0;


    // Implement read event handling
    recv(client->socket, &packet_type, 1, 0);
    //get len
    recv(client->socket, &len, 1, 0);

    switch (packet_type)
    {
    case CONFIG_PACKET:
        //Get the username
        intPacketLen = (int)len;
        if (recvall(client->socket, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
        {
            // Error
            debugError("recv_all");
            return;
        }
        unpack(buffer, "8s", client->username);
        debugLog1_constr("Username: %s\n", client->username);
        if ((client->userID = username_handling(client->username)) == -1) // Add the username or check if existing in database
        {
            debugError("config packet");
            return;
        }
        else
        {
            //set the username to online
            client->status = ONLINE;
            if(status_handling(client->userID,client->status) == -1)  //Update the Online status
            {
                debugError("config packet");
                return;
            }

            //Send list to client
            client->send_msg[0] = CONFIG_PACKET;
            client->config_flag = 1; //Now send the username list to client
        }
        break;
    case CHAT_INIT_PACKET:  //User as sent a UserID to initate chat with other user
        //Recv packet
        recv(client->socket,buffer,len,0);
        unpack(buffer,"h",&client->chat_userID);
        debugLog2("User %s wants to chat with UserID %d\n",client->username,client->chat_userID);
        if(check_status(client->chat_userID) != 1)
        {
            //User Offline, notify client
            client->send_msg[0] = ERROR_PACKET;
            client->error_flag =  1;

        }

        break;
    case MESSAGE_PACKET:
        recv(client->socket,buffer,len,0);
        unpack(buffer,"s",client->rx_msg);
        debugLog2("User %s Rx message for %d: %s\n",client->username,client->chat_userID, client->rx_msg);
        // Need a new set of queue mechanism to handle inter thread messaging
        
        
        if(check_status(client->chat_userID) != 1)
        {
            //User Offline, notify client
            client->send_msg[0] = ERROR_PACKET;
            client->error_flag =  1;

        }
        else
        {
            //UsrID online
            client->msg_flag = 1;
        }
        
        
        break;
    
    default:
        debugError("Invalid Packet\n");
        break;
    }

}

/**
 * @brief Write handler, handles writing to the socket
 * 
 * @param client 
 */
void onWriteHandler(User_Context* client) 
{
    // Implement write event handling
    switch (client->send_msg[0])
    {
    case CONFIG_PACKET:
        if(send_username(client->socket, client->userID ) == -1)
        {
            debugError("write");
        }
        client->config_flag = 0;
        break;
    case CHAT_INIT_PACKET:

        break;
    case MESSAGE_PACKET:
        //Send message to user
        if(send_packet(client->socket,MESSAGE_PACKET,"s",&client->send_msg[1]) == -1)
        {
            soft_error_handle("sendall");
        }
        break;
    case ERROR_PACKET:
        if(client->error_flag == 1) //User not online error
        {
            if(send_packet(client->socket,ERROR_PACKET,"s","User Not Online") == -1)
            {
                soft_error_handle("sendall");
            }
        }
        client->error_flag = 0;
        break;

    default:
        break;
    }

}

/**
 * @brief Exception Handler
 * 
 * @param client 
 */
void onExceptionHandler(User_Context* client) 
{
    // Implement exception event handling
}
