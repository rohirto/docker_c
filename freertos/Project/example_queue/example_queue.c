/**
 * @file example_queue.c
 * @author rohirto
 * @brief Demo of Queue usage in FreeRTOS
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include "example_queue.h"

// Define the queue handle
QueueHandle_t xQueue;

// Task that sends data to the queue
void SenderTask(void *pvParameters) {
    (void) pvParameters;
    int data = 0;

    while (1) {
        // Send data to the queue
        xQueueSend(xQueue, &data, portMAX_DELAY);
        printf("Sent: %d\n", data);

        // Increment data for the next iteration
        data++;

        // Delay for a while
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task that receives data from the queue
void ReceiverTask(void *pvParameters) {
    (void) pvParameters;
    int receivedData;

    while (1) {
        // Receive data from the queue
        if (xQueueReceive(xQueue, &receivedData, portMAX_DELAY) == pdTRUE) {
            printf("Received: %d\n", receivedData);
        }
    }
}