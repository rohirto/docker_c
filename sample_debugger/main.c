/**
 * @file main.c
 * @author rohirto
 * @brief Main File for the sample Debugger
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
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

#else
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "queue.h"
#include "timers.h"

#include "debug_logging.h"
#include "log_to_ringbuffer.h"

#ifdef CH3_TASKMANAGEMENT
void vTask1(void*);
void vTask2(void*);
void vTask3(void*);
void vTask4(void*);

#endif

void vApplicationIdleHook(void);

#endif

void vApplicationIdleHook(void)
{
//	printf("Idle\r\n");
}

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


void vTask1(void* parameter)
{
    
    while(1){
        DBGLOG_INFO("Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
void vTask2(void* parameter)
{
    while(1){
        DBGLOG_INFO("Task 2\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}


void LogReaderTask(void *pvParameters)
{
    uint8_t buffer[128];
    size_t fetchedLength = 0;

    while (1)
    {
        // Call the ring buffer pop function
        LOG_Dump(buffer, 128, &fetchedLength);

        // Check if any data was fetched
        if (fetchedLength > 0)
        {
            // Process the data (example: print it)
            printf("Fetched %d bytes from ring buffer: ", (int)fetchedLength);
            for (size_t i = 0; i < fetchedLength; i++)
            {
                printf("%c", buffer[i]); // Print in hexadecimal format
            }
            printf("\n");
        }

        // Simulate periodic task execution (e.g., 100ms delay)
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


int main()
{
    DBGLOG_Init();
    xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL );
	xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
    xTaskCreate( LogReaderTask, "Task 3", 1000, NULL, 1, NULL );
    vTaskStartScheduler();
    while(1);
    return 0;
}



