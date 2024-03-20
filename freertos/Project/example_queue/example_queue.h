/**
 * @file example_queue.h
 * @author rohirto
 * @brief Header for Queue Module
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EXAMPLE_QUEUE_H
#define EXAMPLE_QUEUE_H

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"

extern QueueHandle_t xQueue;


void SenderTask(void *);
void ReceiverTask(void *);

#endif