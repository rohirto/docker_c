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
#include <ctype.h>
#include <stdlib.h>
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



/**
 * @brief Print Header for Chat
 * @callergraph
 * 
 */
void print_chat_header()
{
    fprintfBlue(stdout,"******************************* USER NAMES ***************************************\n");
    fprintfBlue(stdout,"*Enter the UserID to Chat with (r to Refresh the list, q to quit the application):\n");
    fprintfBlue(stdout,"**********************************************************************************\n");
}

/**
 * @brief Validate the string input based on the params provided 
 * 
 * @param input input string
 * @param limit Limit the no of the chars in a string
 * @param strict If strict is 1 then no numbers and special chars allowed (even space)
 * @return int 1 if Valid, 0 if invalid
 * @callergraph
 */
int isValidInput(const char *input, int limit, int strict) 
{
    int i = 0;
    // Check for special characters or numerals
    if (strict == 1)
    {
        for (i = 0; input[i] != '\0'; i++)
        {
            if (!isalpha(input[i]))
            {
                return 0; // Invalid input
            }
        }
    }

    // Check the length
    if (i > limit) 
    {
        return 0; // Invalid input, length exceeds 8 characters
    }

    return 1; // Valid input
}

/**
 * @brief Validate the string if its a number or not
 * 
 * @param input charcter string
 * @return int 1 if a number, 0 if not a number
 * @callergraph
 */
int isNumber(const char *input) 
{
    char *endptr;
    strtol(input, &endptr, 10);

    // Check for conversion errors
    if (*endptr != '\0' && !isspace((unsigned char)*endptr))
    {
        return 0; // Not a number
    }

    return 1; // It's a number
}

