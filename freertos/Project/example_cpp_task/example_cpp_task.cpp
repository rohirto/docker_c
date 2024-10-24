/**
 * @file example_cpp_task.cpp
 * @author rohirto
 * @brief Module for creating tasks in cpp
 * @version 0.1
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// #include "example_cpp_task.h"
// #include "FreeRTOS.h"
// #include "task.h"
// Wrapping FreeRTOS headers in extern "C" to prevent C++ name mangling
#include <iostream>
#include "example_cpp_task.h"

void * operator new(std::size_t n)
{
  void * const p = std::malloc(n);
  // handle p == 0
  return p;
}

void operator delete(void * p) // or delete(void *, std::size_t)
{
  std::free(p);
}


// Derived class that implements the task function
MyTask::MyTask() : FreeRTOSTask("MyTask", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY + 2) {}

void MyTask::taskFunction() {
    while (true) {
        // Task implementation
        std::cout <<"Running MyTask in Cpp...\n";
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000ms
    }
}
