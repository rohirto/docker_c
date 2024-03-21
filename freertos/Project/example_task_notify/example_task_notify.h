/**
 * @file example_task_notify.h
 * @author rohirto
 * @brief Header File for Task Notify Module
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef EXAMPLE_TASK_NOTIFY_H
#define EXAMPLE_TASK_NOTIFY_H

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "config.h"

extern TaskHandle_t xTask1Handle, xTask2Handle;

void vExampleISR( int signal );
void vTaskt2(void *pvParameters);
void vTaskt1(void *pvParameters);
#endif