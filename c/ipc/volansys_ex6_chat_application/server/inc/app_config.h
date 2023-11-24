/**
 * @file app_config.h
 * @author rohit s
 * @brief Config File for server customization
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

//Customizations
#define USE_SELECT      0   /**< Use Select Functionality in Main thread */
#define USE_SELECT_CLI  1   /**< Use Select Functionality in Client thread */
#define USE_THREADS     1   /**< Use Thread Functionality to implement server */
#define USE_DEBUG       1   /**< Toggle debug messages */


//Defines
#define MAX_USERS       128         /**< Max users which can sign up for Server */
#define PORT "9034"                 /**< Server Port */
#define MAX_LISTEN_BACKLOG      10  /**< Max backlog for Listen */

#if USE_THREADS
#define THREAD_POOL_SIZE    20      /**< Thread Pool Size */
#endif

#define CONFIG_PACKET       0x01    /**< Config Packets - for config purpose - username getting*/
#define CHAT_INIT_PACKET    0x02    /**< Chat init packet - from client to server only, to let server know which user Id to chat with */
#define MESSAGE_PACKET      0x03    /**< Message Packet - Carries message intended for the other client thread */
#define ERROR_PACKET        0x04    /**< Error packets - from server to client only, when some error has occured */


//Static Library declarations
extern unsigned int pack(unsigned char *, char *, ...);
extern void unpack(unsigned char *, char *, ...);

#endif