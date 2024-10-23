/**
 * @file example_static_memory_allocation.c
 * @author rohirto
 * @brief Static memory alloc in FreeRTOS
 * @version 0.1
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "example_static_memory_allocation.h"

// Allocate memory for Idle task's TCB and stack
static StaticTask_t xIdleTaskTCB;
static StackType_t xIdleTaskStack[configMINIMAL_STACK_SIZE];
// Allocate memory for Timer Service task's TCB and stack
static StaticTask_t xTimerTaskTCB;
static StackType_t xTimerTaskStack[configTIMER_TASK_STACK_DEPTH];


// Function to provide memory for Idle task
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    // Provide the memory for the TCB
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    // Provide the memory for the stack
    *ppxIdleTaskStackBuffer = xIdleTaskStack;

    // Provide the stack size
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


// Function to provide memory for the Timer Service task
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    // Provide the memory for the Timer Service task's TCB
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    // Provide the memory for the Timer Service task's stack
    *ppxTimerTaskStackBuffer = xTimerTaskStack;

    // Provide the stack size for the Timer Service task
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}