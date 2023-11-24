/**
 * @file app_debug.c
 * @author rohit S
 * @brief Contains various debug log level functions
 * @version 0.1
 * @date 2023-11-09
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 */

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include "app_debug.h"

// ANSI color codes
#define RED_COLOR "\033[1;31m"      /**< Red color Define */
#define GREEN_COLOR "\033[1;32m"    /**< Green color Define */
#define RESET_COLOR "\033[0m"       /**< Reset color Define */
#define BLUE_COLOR "\033[1;34m"     /**< Blue color Define */

#define ANSI_COLOR_RED     "\x1b[31m"   /**< Ansi Color Red */
#define ANSI_COLOR_RESET   "\x1b[0m"    /**< Ansi color reset */

// Debug log level 1
/**
 * @brief Debug Log Level 1, adds constructive [+] to the log message
 * 
 * @param format standard format string
 * @param ... variable arguments to be formatted into string
 */
void _debugLog1_constr(const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    printf(GREEN_COLOR);
    printf("[+] ");
    printf(RESET_COLOR);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}
/**
 * @brief Debug Log Level 1, adds destructive [-] to the log message
 * 
 * @param format standard format string
 * @param ... variable arguments to be formatted into string
 */
void _debugLog1_destr(const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    printf(RED_COLOR);
    printf("[-] ");
    printf(RESET_COLOR);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}

// Debug log level 2
/**
 * @brief Debug Log Level 2, adds destructive ==> to the log message
 * 
 * @param format standard format string
 * @param ... variable arguments to be formatted into string
 */
void _debugLog2(const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    printf(BLUE_COLOR);
    printf(" ==> ");
    printf(RESET_COLOR);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}

/**
 * @brief Debug Error
 * 
 * @param prefix 
 */
void _debugError(const char *prefix)
{
    fprintf(stderr," ==> %s%s: %s%s\n", RED_COLOR, prefix, strerror(errno), RESET_COLOR);

}

/**
 * @brief Function to print blue text to a file pointer
 * 
 * @param file 
 * @param format 
 * @param ... 
 */
void _fprintfBlue(FILE *file, const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    fprintf(file,BLUE_COLOR" ==> ");
    vfprintf(file,format,args);
    fprintf(file,RESET_COLOR);
    fflush(file);
    va_end(args);
}

/**
 * @brief Function to print Red text to a file pointer
 * 
 * @param file 
 * @param format 
 * @param ... 
 */
void _fprintfRed(FILE *file, const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    fprintf(file,RED_COLOR" ==> ");
    vfprintf(file,format,args);
    fprintf(file,RESET_COLOR);
    fflush(file);
    va_end(args);
}

/**
 * @brief Function to print Green text to a file pointer
 * 
 * @param file 
 * @param format 
 * @param ... 
 */
void _fprintfGreen(FILE *file, const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    fprintf(file,GREEN_COLOR" ==> ");
    vfprintf(file,format,args);
    fprintf(file,RESET_COLOR);
    fflush(file);
    va_end(args);
}

