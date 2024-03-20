/**
 * @file config.h
 * @author rohirto
 * @brief Configuration File for Application layer of Project
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CONFIG_H
#define CONFIG_H


//#define USE_QUEUES
//#define USE_TIMERS
#define USE_SEMAPHORE
#ifndef USE_SEMAPHORE
#define USE_COUNT_SEMAPHORE  //Use either Binary sem or Counting sem

#endif

#endif