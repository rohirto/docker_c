/**
 * @file app_queue.c
 * @author rohit S
 * @brief Data structure linked list to handle the incoming client connections
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 *  Linked List implementation of Queue to Faciliate Inter thread communications
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "app_queue.h"

node_t *head = NULL;    /**< Head of Client Queue */
node_t *tail = NULL;    /**< Tail of Client Queue */


t_comm *t_head = NULL;  /**< Head of Message Queue */
t_comm *t_tail = NULL;  /**< Tail of Message Queue */


/**
 * @brief Enqueue the incoming client context to a queue, always enqueued by listener thread
 * 
 * @param client 
 * @callergraph
 */
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

/**
 * @brief Dequeue the incoming client context, alwaays dequeued by the client threads
 * 
 * @return User_Context* 
 * @callergraph
 */
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

/**
 * @brief Enqueue a message intended for a client thread, always enqueued by chat sender thread
 * 
 * @param message 
 * @callergraph
 */
void enqueue_msg(msg_t* message)
{
    t_comm *newnode = malloc(sizeof(t_comm));
    newnode->msgx = malloc(sizeof(msg_t));
    //newnode->msgx = message;
    memcpy(newnode->msgx, message, sizeof(msg_t));
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

/**
 * @brief Dequeue a message intended for a client thread, always dequeued by chat receiver thread
 * 
 * @param userID User ID of receiver, if matched, only then accept the message, else leave it
 * @return msg_t* returns the message struct
 * @callergraph
 */
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