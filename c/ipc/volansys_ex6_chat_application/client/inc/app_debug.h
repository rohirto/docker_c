/**
 * @file app_debug.h
 * @author rohit s
 * @brief This file is a user include and gives access to functions of debug
 * @version 0.1
 * @date 2023-11-09
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 */
#ifndef APP_DEBUG_H
#define APP_DEBUG_H

#include <stdio.h>

//Function Prototypes
void debugLog1_constr(const char *format, ...); 
void debugLog1_destr(const char *format, ...);
void debugLog2(const char *format, ...);
void debugError(const char *prefix);
void fprintfBlue(FILE *file, const char *format, ...);
void fprintfRed(FILE *file, const char *format, ...);
void fprintfGreen(FILE *file, const char *format, ...);



#endif