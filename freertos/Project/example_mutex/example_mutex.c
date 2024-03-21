/**
 * @file example_mutex.c
 * @author rohirto
 * @brief Mutex Module of FreeRTOS
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include "example_mutex.h"

// Define a mutex handle
SemaphoreHandle_t xMutex;

// Task 1
void vTaskm1(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        // Attempt to take the mutex
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            // Mutex taken, critical section
            // Perform task-specific actions here
            // For example, toggle an LED

            // Release the mutex when done with the critical section
            xSemaphoreGive(xMutex);
        }

        // Simulate task execution with a delay
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Task 2
void vTaskm2(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        // Attempt to take the mutex
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            // Mutex taken, critical section
            // Perform task-specific actions here
            // For example, print to the console
            printf("Task 2 executed\n");

            // Release the mutex when done with the critical section
            xSemaphoreGive(xMutex);
        }

        // Simulate task execution with a delay
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}