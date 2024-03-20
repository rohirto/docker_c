/**
 * @file example_semaphore.h
 * @author rohirto
 * @brief Header File for Semaphore Module
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef EXAMPLE_SEMAPHORE_H
#define EXAMPLE_SEMAPHORE_H

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"
#include "semphr.h"

extern SemaphoreHandle_t xSemaphore;

void vTask_sem_1(void *);
void vTask_sem_2(void *);

#endif