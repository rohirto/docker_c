/**
 * @file example_cpp_task.h
 * @author rohirto
 * @brief Header module for module for creating tasks in cpp
 * @version 0.1
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EXAMPLE_CPP_TASK_H
#define EXAMPLE_CPP_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}
#endif

class FreeRTOSTask {
public:
    FreeRTOSTask(const char* name, uint16_t stackDepth, UBaseType_t priority):
        taskHandle(nullptr), taskName(name), taskStackDepth(stackDepth), taskPriority(priority) {}
    virtual ~FreeRTOSTask(){
        if (taskHandle != nullptr) {
            vTaskDelete(taskHandle); // Delete the task when the object is destroyed
        }
    }
    void start() {
        // Static function is passed as the task function
        xTaskCreate(taskFunctionWrapper, taskName, taskStackDepth, this, taskPriority, &taskHandle);
    }

protected:
    virtual void taskFunction() = 0;

private:
    TaskHandle_t taskHandle;
    const char* taskName;
    uint16_t taskStackDepth;
    UBaseType_t taskPriority;

    // Static function wrapper for the FreeRTOS task function
    static void taskFunctionWrapper(void* pvParameters) {
        // Cast the parameter back to the FreeRTOSTask object
        FreeRTOSTask* task = static_cast<FreeRTOSTask*>(pvParameters);
        task->taskFunction(); // Call the virtual method
    }
};

class MyTask : public FreeRTOSTask {
public:
    MyTask();
protected:
    void taskFunction() override;
};
#endif