/**
 * @file example_timers.c
 * @author rohirto
 * @brief Timer Module for FreeRTOS module
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include "example_timers.h"



// Timer callback function
void TimerCallback(TimerHandle_t xTimer) {
    // Perform actions when the timer expires
    printf("Timer expired!\n");
}

// Task that creates the software timer
void TimerTask(void *pvParameters) {
    // Create the software timer with a period of 1000 milliseconds
    xTimer = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, 0, TimerCallback);

    // Check if the timer was created successfully
    if (xTimer != NULL) {
        // Start the timer
        if (xTimerStart(xTimer, 0) != pdPASS) {
            printf("Failed to start timer!\n");
        }
    } else {
        printf("Failed to create timer!\n");
    }

    // Task does not need to do anything further
    vTaskDelete(NULL);
}