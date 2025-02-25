/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * main() creates all the demo application tasks, then starts the scheduler.
 * The web documentation provides more details of the standard demo application
 * tasks, which provide no particular functionality but do provide a good
 * example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" task - This only executes every five seconds but has a high priority
 * to ensure it gets processor time.  Its main function is to check that all the
 * standard demo tasks are still operational.  While no errors have been
 * discovered the check task will print out "OK" and the current simulated tick
 * time.  If an error is discovered in the execution of a task then the check
 * task will print out an appropriate error message.
 *
 */

#ifdef __cplusplus
extern "C" {
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"
#include "example_queue.h"
#include "example_timers.h"
#include "example_semaphore.h"
#include "example_mutex.h"
#include "example_task_notify.h"
#include "example_event_group.h"
#include "example_recursive_mutex.h"
#include "config.h"
//#include "queue.h"
/* Examples */
//#define CH3_TASKMANAGEMENT

/* --------------------------------------------- */
#ifdef CH3_TASKMANAGEMENT
void vTask1(void*);
void vTask2(void*);
void vTask3(void*);
void vTask4(void*);

#endif



void vApplicationIdleHook(void);

}
#endif

#include "example_cpp_task.h"

void cpp_task_creator()
{
    MyTask myTask;

    myTask.start();
}

#ifdef __cplusplus
extern "C" {
#endif
int main ( void )
{
    
#ifdef USE_INTERRUPT
    /* SIGINT is not blocked by the posix port */
    signal( SIGINT, vExampleISR );
#endif
#ifdef CH3_TASKMANAGEMENT
	/* Creating Two Task Same Priorities and Delay*/
	//xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL );
	//xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
	/* Creating Two Task Same Priorities and DelayUntil*/
	xTaskCreate( vTask3, "Task 3", 1000, NULL, 1, NULL );
	xTaskCreate( vTask4, "Task 4", 1000, NULL, 1, NULL );
#endif
#ifdef USE_QUEUES
    // Create the queue
    xQueue = xQueueCreate(5, sizeof(int));
    // Create the sender and receiver tasks
    xTaskCreate(SenderTask, "Sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(ReceiverTask, "Receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#endif
#ifdef USE_TIMERS
    // Create the timer task
    xTaskCreate(TimerTask, "TimerTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#endif
#ifdef USE_SEMAPHORE
    // Create the semaphore
    vSemaphoreCreateBinary(xSemaphore);

    if(xSemaphore != NULL) {
        // Create Task 1
        xTaskCreate(vTask_sem_1, "T1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

        // Create Task 2
        xTaskCreate(vTask_sem_2, "T2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    }
    else
    {
        printf("Cannot create Seamphore \n");
    }
#endif

#ifdef USE_COUNT_SEMAPHORE
    xSemaphore=xSemaphoreCreateCounting(3,0);
    if(xSemaphore != NULL) {
        // Create Task 1
        xTaskCreate(vTask_sem_1, "T1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Create Task 2
        xTaskCreate(vTask_sem_2, "T2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    }
    else
    {
        printf("Cannot create Seamphore \n");
    }

#endif

#ifdef USE_MUTEX
// Create a mutex
    #ifndef USE_STATIC_MUTEX
    xMutex = xSemaphoreCreateMutex();
    #else
    // Create the static mutex
    xMutex = xSemaphoreCreateMutexStatic(&xMutexBuffer);
    #endif

    // Create tasks
    xTaskCreate(vTaskm1, "Task m1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTaskm2, "Task m2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#endif

#ifdef USE_TASK_NOTIFY
// Create Task 1
    xTaskCreate(vTaskt1, "Task t1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTask1Handle);

    // Create Task 2
    xTaskCreate(vTaskt2, "Task t2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTask2Handle);
#endif

#ifdef USE_EVENT_GROUP
    // Create an event group
    eventGroup = xEventGroupCreate();

    // Create tasks
    xTaskCreate(vTaskeg1, "Taskeg1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTaskeg2, "Taskeg2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

#endif

#ifdef USE_RECURSIVE_MUTEX
    xRecursiveMutex = xSemaphoreCreateRecursiveMutex();
    if(xRecursiveMutex == NULL)
    {
        printf("Failed to create the recursive Mutex.\n");
        return 1;
    }

    xTaskCreate(TaskRM_A, "Task A", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(TaskRM_B, "Task B", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#endif

#ifdef __cplusplus
    MyTask myTask; // Creating the C++ object

    // Start the task using the C++ method
    myTask.start(); // Start the FreeRTOS task
#endif



	vTaskStartScheduler();
	return 0;
}

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
 	taskENTER_CRITICAL();
	{
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


#ifdef CH3_TASKMANAGEMENT
void vTask1(void* parameter)
{
    while(1){
        printf("Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(250));
    }
}
void vTask2(void* parameter)
{
    while(1){
        printf("Task 2\n");
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 3 with 250ms\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(250));
    }
}
void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 4 with 500ms\n");
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(500));
    }
}

#endif
/* CH3_TASKMANAGEMENT ends */


void vApplicationIdleHook(void)
{
//	printf("Idle\r\n");
}


#ifdef USE_INTERRUPT
// ISR
void vExampleISR( int signal ) {
    (void) signal;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
#ifdef USE_TASK_NOTIFY
    static uint32_t ulISRValue = 0;
    ulISRValue++;
    // Notify Task 1 with a value from ISR
    xTaskNotifyFromISR(xTask1Handle, ulISRValue,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);

    // Perform other ISR actions
    printf("Notify from Interrupt: %u\n",ulISRValue );

#endif
#ifdef USE_EVENT_GROUP
    // Set event bits from ISR
    xEventGroupSetBitsFromISR(eventGroup, EVENT_BIT_1 | EVENT_BIT_2, &xHigherPriorityTaskWoken);

#endif

    // Check if a higher priority task was woken
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

#ifdef __cplusplus
}
#endif

/*-----------------------------------------------------------*/
