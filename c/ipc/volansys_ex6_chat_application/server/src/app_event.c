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




// Function to dispatch events
int dispatchEvent(User_Context* client, EventType eventType, EventHandler* eventHandler) 
{
    switch (eventType)
    {
    case READ_EVENT:
        if (eventHandler->onRead != NULL)
        {
            unsigned char c;
            int bytesRead;
            if ((bytesRead = recv(client->socket, &c, 1, MSG_PEEK)) == -1)
            {
                debugError("Socket error\n");
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
                return -1;
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

// Event handler functions
void onReadHandler(User_Context* client) 
{
    unsigned char packet_type;
    unsigned char len;
    unsigned char buffer[128];
    int bytesRead;


    // Implement read event handling
    bytesRead = recv(client->socket, &packet_type, 1, 0);
    //get len
    bytesRead = recv(client->socket, &len, 1, 0);

    switch (packet_type)
    {
    case CONFIG_PACKET:
        //Get the username
        bytesRead = recv(client->socket,buffer,len,0);
        memset(client->username, 0x00, 8);
        memcpy(client->username,buffer, len);
        debugLog1_constr("Username: %s\n",client->username);
        if((client->userID = username_handling(client->username)) == -1)
        {
            debugError("config packet");
        }
        else
        {
            //set the username to online
            client->status = ONLINE;
            if(status_handling(client->userID,client->status) == -1)
            {
                debugError("config packet");
            }

            //Send list to client
            client->send_msg[0] = CONFIG_PACKET;
            client->send_flag = 1; 
        }
        break;
    case CHAT_INIT_PACKET:  //User as sent a UserID to initate chat with other user
        //Recv packet
        bytesRead = recv(client->socket,buffer,len,0);
        unpack(buffer,"h",&client->chat_userID);
        debugLog2("User %s wants to chat with UserID %d\n",client->username,client->chat_userID);
        break;
    case MESSAGE_PACKET:
        bytesRead = recv(client->socket,buffer,len,0);
        unpack(buffer,"s",client->rx_msg);
        debugLog2("User %s Rx message for %d: %s\n",client->username,client->chat_userID, client->rx_msg);
        // Need a new set of queue mechanism to handle inter thread messaging
        break;
    
    default:
        debugError("Invalid Packet\n");
        break;
    }
    
    // else if (bytesRead == 0)
    // {
    //     close(client->socket);
    //     debugLog2("%s","Socket closed");
    // }
    // else
    // {
    //     debugLog2("%s","Scoket data");
    // }

}

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
        client->send_flag = 0;
        break;
    case CHAT_INIT_PACKET:

        break;
    case MESSAGE_PACKET:
        break;
    
    default:
        break;
    }

}

void onExceptionHandler(User_Context* client) 
{
    // Implement exception event handling
}
