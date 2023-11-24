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
#define USE_DEBUG 1


//Macros
#if USE_DEBUG == 1
#define debugLog1_constr(fmt, ...) _debugLog1_constr(fmt, ##__VA_ARGS__)
#define debugLog1_destr(fmt, ...) _debugLog1_destr(fmt, ##__VA_ARGS__)
#define debugLog2(fmt, ...) _debugLog2(fmt, ##__VA_ARGS__)
#define debugError(fmt) _debugError(fmt)
#define fprintfBlue(file, fmt, ...) _fprintfBlue(file,fmt,##__VA_ARGS__)
#define fprintfRed(file, fmt, ...) _fprintfRed(file,fmt,##__VA_ARGS__)
#define fprintfGreen(file, fmt, ...) _fprintfGreen(file,fmt,##__VA_ARGS__)
#else

#define debugLog1_constr(fmt, ...) 
#define debugLog1_destr(fmt, ...) 
#define debugLog2(fmt, ...) 
#define debugError(fmt) 
#define fprintfBlue(file, fmt, ...) 
#define fprintfRed(file, fmt, ...) 
#define fprintfGreen(file, fmt, ...) 

#endif

//Function Prototypes
void debugLog1_constr(const char *format, ...); 
void debugLog1_destr(const char *format, ...);
void debugLog2(const char *format, ...);
void debugError(const char *prefix);
void fprintfBlue(FILE *file, const char *format, ...);
void fprintfRed(FILE *file, const char *format, ...);
void fprintfGreen(FILE *file, const char *format, ...);
void print_chat_header();
int isValidInput(const char *, int ,int);
int isNumber(const char *);


#endif