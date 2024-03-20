/**
 * @file example_timers.h
 * @author Header File for Timer module
 * @brief 
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EXAMPLE_TIMER_H
#define EXAMPLE_TIMER_H

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"

// Define the software timer handle
extern TimerHandle_t xTimer;

void TimerTask(void *pvParameters);
void TimerCallback(TimerHandle_t);

#endif