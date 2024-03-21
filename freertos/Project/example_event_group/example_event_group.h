/**
 * @file example_event_group.h
 * @author rohirto
 * @brief Header file for Event Group Module
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EXAMPLE_EVENT_GROUP_H
#define EXAMPLE_EVENT_GROUP_H


#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

// Define event bit definitions
#define EVENT_BIT_1     (1 << 0)
#define EVENT_BIT_2     (1 << 1)


// Create an event group handle
extern EventGroupHandle_t eventGroup;


// Task function prototypes
void vTaskeg1(void *pvParameters);
void vTaskeg2(void *pvParameters);
#endif