/**
 * @file example_recursive_mutex.c
 * @author rohirto 
 * @brief Source File for recursive Mutex Demo
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "example_recursive_mutex.h"

SemaphoreHandle_t xRecursiveMutex;

void TaskRM_A(void *pvParams)
{
    (void) pvParams;
    //Acquire R Mutex
    if (xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY))
    {
        printf("Task A has acquired the Recursive Mutex for first time\n");

        //Acquire R Mutex for second time
        if(xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY))
        {
            printf("Task A has acquired the Recursive Mutex for second time\n");

            //Simulate some work
            vTaskDelay(pdMS_TO_TICKS(1000));

            //Give the Mutex Second time
            xSemaphoreGiveRecursive(xRecursiveMutex);
            printf("Task A has given Recursive Mutex For second time\n");
        }

        xSemaphoreGiveRecursive(xRecursiveMutex);
        printf("Task A has given Recursive Mutex For first time\n");

        
    }
    vTaskDelete(NULL);
}

void TaskRM_B(void *pvParams)
{
    (void) pvParams;
    //Acquire R Mutex
    if (xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY))
    {
        printf("Task B has acquired the Recursive Mutex for first time\n");

        //Acquire R Mutex for second time
        if(xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY))
        {
            printf("Task B has acquired the Recursive Mutex for second time\n");

            //Simulate some work
            vTaskDelay(pdMS_TO_TICKS(1000));

            //Give the Mutex Second time
            xSemaphoreGiveRecursive(xRecursiveMutex);
            printf("Task B has given Recursive Mutex For second time\n");
        }

        xSemaphoreGiveRecursive(xRecursiveMutex);
        printf("Task B has given Recursive Mutex For first time\n");

        
    }
    vTaskDelete(NULL);
}