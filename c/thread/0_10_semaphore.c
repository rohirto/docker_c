/**
 * In example 0_7_mutexs.c, If the job queue becomes empty, the thread exits and the application ends
 * But what if we anticipate that further jobs will be added in this queue and thread must go into a blocking state and execute tasks again 
 * when new jobs arrive
 * 
 * To acheive above task semaphores are used
 * every Semaphr has a counter value
 * Supports 2 operation 
 *      1. Wait op - decrement the counter by 1 till it becomes 0, block threads from here, until the counter again becomes positive
 *      2. Post op - increments the value of counter by 1
 * 2 implementations
 *      1. Communication among threads  - explained here
 *      2. Communication among Processes
*/
