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

// Function to dispatch events
void dispatchEvent(User_Context* client, EventType eventType, EventHandler* eventHandler) 
{
    switch (eventType)
    {
        case READ_EVENT:
            if (eventHandler->onRead != NULL) {
                eventHandler->onRead(client);
            }
            break;
        case WRITE_EVENT:
            if (eventHandler->onWrite != NULL) {
                eventHandler->onWrite(client);
            }
            break;
        case EXCEPTION_EVENT:
            if (eventHandler->onException != NULL) {
                eventHandler->onException(client);
            }
            break;
        default:
            // Handle unknown event type
            break;
    }
}

// Event handler functions
void onReadHandler(User_Context* client) 
{
    unsigned char buffer[1024];
    int bytesRead;
    // Implement read event handling
    debugLog2("Reached till here");
    
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
}

void onExceptionHandler(User_Context* client) 
{
    // Implement exception event handling
}
