/**
 * @file example_task_notify.c
 * @author rohirto
 * @brief Task notify Module
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include "example_task_notify.h"

// Task handles
TaskHandle_t xTask1Handle, xTask2Handle;

// Task 1
void vTaskt1(void *pvParameters) {
    uint32_t ulNotifiedValue;
    while (1) {
        // Wait for a notification from Task 2 or ISR
        xTaskNotifyWait(0x00, /* Don't clear any notification bits on entry. */
                        UINT32_MAX, /* Reset the notification value to 0 on exit. */
                        &ulNotifiedValue, /* Notified value pass out in ulNotifiedValue. */
                        portMAX_DELAY );  /* Block indefinitely. */
    

        // Task 1 specific actions based on the notified value
        // For example, toggle an LED based on the notified value
        printf("Task1 Notification Received: %u\n", ulNotifiedValue);
    }
}

// Task 2
void vTaskt2(void *pvParameters) {
    static uint32_t ulSensorValue = 0;
    while (1) {
        // Task 2 specific actions
        // For example, read sensor data and determine the notified value
        ulSensorValue++;
        // Notify Task 1 with a value
        xTaskNotify(xTask1Handle, ulSensorValue, eSetValueWithOverwrite);
        printf("Task 2 Notifying task 1 with value: %u\n", ulSensorValue);

        // Task 2 delay before next notification
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#ifdef USE_INTERRUPT
// ISR
void vExampleISR( int signal ) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulISRValue = 0;
    ulISRValue++;
    // Notify Task 1 with a value from ISR
    xTaskNotifyFromISR(xTask1Handle, ulISRValue,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);

    // Perform other ISR actions
    printf("Notify from Interrupt: %u\n",ulISRValue );

    // Check if a higher priority task was woken
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif