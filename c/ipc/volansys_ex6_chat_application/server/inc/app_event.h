/**
 * @file app_event.h
 * @author rohit S
 * @brief header file for app_event.c
 * @version 0.1
 * @date 2023-11-12
 * 
 * @copyright Copyright (c) 2023 Volansys Yechnologies
 * 
 */
#ifndef APP_EVENT_H
#define APP_EVENT_H

#include <pthread.h>

/**
 * @brief User context struct
 * 
 */
typedef struct user_cntxt
{
    unsigned char userID;
    unsigned char username[8];  //8 Bytes username
    unsigned char password[8];  //8 byte password
    unsigned char status; //0x00 Offline, 0x01 Online
    unsigned char send_msg[128];
    unsigned char rx_msg[128];

    int socket;
    pthread_t threadID;

}User_Context;

// Define event types
typedef enum 
{
    READ_EVENT,
    WRITE_EVENT,
    EXCEPTION_EVENT
} EventType;

// Event callback function signatures
typedef void (*ReadEventHandler)(User_Context*);
typedef void (*WriteEventHandler)(User_Context*);
typedef void (*ExceptionEventHandler)(User_Context*);

// Structure to hold event handlers
typedef struct 
{
    ReadEventHandler onRead;
    WriteEventHandler onWrite;
    ExceptionEventHandler onException;
} EventHandler;


//Handler Prototypes
void onReadHandler(User_Context* ) ;
void onWriteHandler(User_Context* ) ;
void onExceptionHandler(User_Context* );

//Event dispatcher prototype
void dispatchEvent(User_Context* , EventType , EventHandler* );

#endif