/**
 * @file logger_config.h
 * @author rohirto
 * @brief 
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H


#ifndef LOG_ENABLE
#define LOG_ENABLE 0
#endif

/*! @brief Whether enable log color global feature, 1 - enable, 0 - disable.
 *  @details The feature is used to configure the log color feature for all of log component.@n
 * The feature should be defined in project setting.@n
 * Below shows how to configure in your project if you want to disable the feature.@n
 * For IAR, right click project and select "Options", define it in "C/C++ Compiler->Preprocessor->Defined symbols".@n
 * For KEIL, click "Options for Target...", define it in "C/C++->Preprocessor Symbols->Define".@n
 * For ARMGCC, open CmakeLists.txt and add the following lines,@n
 * "SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLOG_ENABLE_COLOR=0")" for debug target.@n
 * "SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLOG_ENABLE_COLOR=0")" for release target.@n
 * For MCUxpresso, right click project and select "Properties", define it in "C/C++ Build->Settings->MCU C
 * Complier->Preprocessor".@n
 */
#ifndef LOG_ENABLE_COLOR
#define LOG_ENABLE_COLOR 1
#endif

/*! @brief Whether enable timestamp global feature for log, 1 - enable, 0 - disable.
 * @details The feature is used to configure the log timestamp feature for all of log component.@n
 * The feature should be defined in project setting.@n
 * Below shows how to configure in your project if you want to disable the feature.@n
 * For IAR, right click project and select "Options", define it in "C/C++ Compiler->Preprocessor->Defined symbols".@n
 * For KEIL, click "Options for Target...", define it in "C/C++->Preprocessor Symbols->Define".@n
 * For ARMGCC, open CmakeLists.txt and add the following lines,@n
 * "SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLOG_ENABLE_TIMESTAMP=0")" for debug target.@n
 * "SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLOG_ENABLE_TIMESTAMP=0")" for release target.@n
 * For MCUxpresso, right click project and select "Properties", define it in "C/C++ Build->Settings->MCU C
 * Complier->Preprocessor".@n
 */
#ifndef LOG_ENABLE_TIMESTAMP
#define LOG_ENABLE_TIMESTAMP 1
#endif

#ifndef LOG_ENABLE_PRINT_LEVEL
#define LOG_ENABLE_PRINT_LEVEL 0
#endif

/*! @brief Set the max message length, the default value is 128.
 * @details The feature is used to set the max message length, the default value is 128.@n
 * The feature should be defined in project setting.@n Below shows how to configure in your project if you
 * want to enable the feature.@n For IAR, right click project and select "Options", define it in "C/C++
 * Compiler->Preprocessor->Defined symbols".@n For KEIL, click "Options for Target...", define it in
 * "C/C++->Preprocessor Symbols->Define".@n For ARMGCC, open CmakeLists.txt and add the following lines,@n
 * "SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLOG_MAX_MEESSAGE_LENGTH=128")" for debug target.@n
 * "SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLOG_MAX_MEESSAGE_LENGTH=128")" for release target.@n
 * For MCUxpresso, right click project and select "Properties", define it in "C/C++ Build->Settings->MCU C
 * Complier->Preprocessor".@n
 */
#ifndef LOG_MAX_MEESSAGE_LENGTH
#define LOG_MAX_MEESSAGE_LENGTH 128
#endif


#endif 
