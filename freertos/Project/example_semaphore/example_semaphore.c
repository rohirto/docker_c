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

// Define a semaphore handle
SemaphoreHandle_t xSemaphore;

// Task 1
void vTask_sem_1(void *pvParameters) {
    (void) pvParameters;
    int semcount = 0;
#ifdef USE_COUNT_SEMAPHORE
    xSemaphoreGive(xSemaphore);
    xSemaphoreGive(xSemaphore);
    xSemaphoreGive(xSemaphore);
#endif
    
    while(1) {
        semcount = uxSemaphoreGetCount(xSemaphore);
        printf("Sem Count: %d\n", semcount);

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
    (void) pvParameters;
    int semcount = 0;
    while(1) {
        semcount = uxSemaphoreGetCount(xSemaphore);
        printf("Sem Count: %d\n", semcount);
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