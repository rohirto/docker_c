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
#define USE_SELECT      0
#define USE_SELECT_CLI  1
#define USE_THREADS     1


//Defines
#define MAX_USERS       128
#define PORT "9034" // port we're listening on
#define MAX_LISTEN_BACKLOG      10

#if USE_THREADS
#define THREAD_POOL_SIZE    20
#endif

#define CONFIG_PACKET       0x01
#define CHAT_INIT_PACKET    0x02
#define MESSAGE_PACKET      0x03
#define ERROR_PACKET        0x04


//Static Library declarations
extern unsigned int pack(unsigned char *, char *, ...);
extern void unpack(unsigned char *, char *, ...);

#endif