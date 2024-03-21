/**
 * @file example_event_group.c
 * @author rohirto
 * @brief Event Group Module
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include "example_event_group.h"




// Create an event group handle
EventGroupHandle_t eventGroup;

void vTaskeg1(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        // Wait for EVENT_BIT_1 to be set
        EventBits_t bits = xEventGroupWaitBits(eventGroup, EVENT_BIT_1, pdTRUE, pdTRUE, portMAX_DELAY);

        // Check if EVENT_BIT_1 is set
        if ((bits & EVENT_BIT_1) != 0) {
            // Event bit 1 is set, do something
            printf("Task 1: Event Bit 1 is set\n");
        }
    }
}

void vTaskeg2(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        // Wait for EVENT_BIT_2 to be set with a timeout of 100 ticks
        EventBits_t bits = xEventGroupWaitBits(eventGroup, EVENT_BIT_2, pdTRUE, pdTRUE, pdMS_TO_TICKS(100));

        // Check if EVENT_BIT_2 is set
        if ((bits & EVENT_BIT_2) != 0) {
            // Event bit 2 is set, do something
            printf("Task 2: Event Bit 2 is set\n");
        }
    }
}

