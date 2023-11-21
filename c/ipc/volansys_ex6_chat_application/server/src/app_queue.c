/**
 * @file app_queue.c
 * @author rohit S
 * @brief Data structure linked list to handle the incoming client connections
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 * 
 * 
 */

#include <stdlib.h>
#include "app_queue.h"

node_t *head = NULL;
node_t *tail = NULL;


t_comm *t_head = NULL;
t_comm *t_tail = NULL;


void enqueue(User_Context *client)
{
    node_t *newnode = malloc(sizeof(node_t));

    newnode->client = client;
    newnode->next = NULL;

    if(tail == NULL)
    {
        head = newnode;
    }
    else
    {
        tail->next = newnode;
    }
    tail = newnode;
    free(client);
}

User_Context* dequeue()
{
    if(head == NULL)
    {
        return NULL;

    }
    else
    {
        User_Context* result = head->client;
        node_t *temp = head;
        head = head->next;
        if(head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        return result;
    }
}

void enqueue_msg(msg_t* message)
{
    t_comm *newnode = malloc(sizeof(t_comm));

    newnode->msgx = message;
    newnode->next = NULL;

    if(t_tail == NULL)
    {
        t_head = newnode;
    }
    else
    {
        t_tail->next = newnode;
    }
    t_tail = newnode;
    free(message);
    
}

msg_t* dequeue_msg(int userID)  //If userID matches then its for you
{
    if(t_head == NULL)
    {
        return NULL;

    }
    else if(t_head->msgx->userID == userID)
    {
        msg_t* result = t_head->msgx;
        t_comm *temp = t_head;
        t_head = t_head->next;
        if(t_head == NULL)
        {
            t_tail = NULL;
        }
        free(temp);
        return result;
    }
    else
    {
        return NULL;
    }
}