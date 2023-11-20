/**
 * @file app_queue.h
 * @author rohit s
 * @brief Header file for data structure linked list
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef APP_QUEUE_H
#define APP_QUEUE_H

#include <app_event.h>

struct node
{
    struct node* next;
    User_Context *client;

};
typedef struct node node_t;


//Function Prototypes
void enqueue(User_Context *);
User_Context* dequeue();

#endif