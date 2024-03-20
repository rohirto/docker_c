/**
 * @file example_semaphore.c
 * @author rohirto
 * @brief Semaphore Module
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include "example_semaphore.h"

// Task 1
void vTask_sem_1(void *pvParameters) {
    while(1) {
        // Attempt to take the semaphore
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            // Semaphore taken, critical section begins
            printf("Task 1: Semaphore taken\n");

            // Simulate some processing time
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            // Critical section ends, release the semaphore
            xSemaphoreGive(xSemaphore);
            printf("Task 1: Semaphore released\n");
        }

        // Delay before trying again
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Task 2
void vTask_sem_2(void *pvParameters) {
    while(1) {
        // Attempt to take the semaphore
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            // Semaphore taken, critical section begins
            printf("Task 2: Semaphore taken\n");

            // Simulate some processing time
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            // Critical section ends, release the semaphore
            xSemaphoreGive(xSemaphore);
            printf("Task 2: Semaphore released\n");
        }

        // Delay before trying again
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}