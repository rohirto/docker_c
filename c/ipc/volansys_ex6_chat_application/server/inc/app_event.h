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
#include "app_config.h"

/**
 * @brief User context struct
 * 
 */
typedef struct user_cntxt
{
    int userID;                 /**< USer ID of that User Context */
    char username[9];           /**< User name - can be max 8 bytes */
    //password[8];  //8 byte password
    int status;                 /**< Status - 1 Online, 0 Offline */
    char send_msg[140];         /**< Send message buffer of User Context */
    unsigned char rx_msg[128];  /**< Rx message buffer of User Context */
    int chat_userID;            /**< Current UserID with whom this User is chatting, 255 signifies no active user Id*/

    int socket;                 /**< Scket FD of the USer*/
#if USE_THREADS
    int threadID;               /**< Thread ID, 0 to 20 */
#endif

    int first_flag;             /**< 255 when first initialized, 0 when the user has been sent username list even once */
    int config_flag;            /**< Set when some config packet is received and needs to be processesd*/
    int msg_flag;               /**< Set when some message packet is received */
    int error_flag;             /**< Set when some error flag is received */

}User_Context;


/**
 * @brief Enum of Event types
 * 
 */
typedef enum 
{
    READ_EVENT,             /**< Read Event */
    WRITE_EVENT,            /**< Write Event */
    EXCEPTION_EVENT         /**< Exception Event */
} EventType;

// Event callback function signatures
typedef void (*ReadEventHandler)(User_Context*);
typedef void (*WriteEventHandler)(User_Context*);
typedef void (*ExceptionEventHandler)(User_Context*);

// Structure to hold event handlers
/**
 * @brief struct where callback handlers are defined
 * 
 */
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
int dispatchEvent(User_Context* , EventType , EventHandler* );


#endif