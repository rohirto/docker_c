/**
 * @file example_mutex.h
 * @author rohirto
 * @brief Header file for Mutex Module
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EXAMPLE_MUTEX_H
#define EXAMPLE_MUTEX_H

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"
#include "config.h"
#include "semphr.h"


// Define a mutex handle
extern SemaphoreHandle_t xMutex;


void vTaskm1(void *pvParameters);
void vTaskm2(void *pvParameters);


#endif