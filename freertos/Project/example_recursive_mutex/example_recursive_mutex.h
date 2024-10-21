/**
 * @file example_recursive_mutex.h
 * @author rohirto
 * @brief Header file for recursive mutex demo
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EXAMPLE_RECURSIVE_MUTEX
#define EXAMPLE_RECURSIVE_MUTEX

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include <stdio.h>

extern SemaphoreHandle_t xRecursiveMutex;

void TaskRM_A(void *pvParams);
void TaskRM_B(void *pvParams);

#endif