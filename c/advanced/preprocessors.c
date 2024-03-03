/**
 * @file preprocessors.c
 * @author rohirto
 * @brief code to demonstrate the capabilities of preprocessors in C
 * @version 0.1
 * @date 2023-11-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>

// Simple macro for squaring a number
#define SQUARE(x) ((x) * (x))

// Define or undefine FEATURE_A to include or exclude specific code blocks
#define FEATURE_A

// String concatenation
#define CONCAT(a, b) a ## b

// Token pasting
#define ADD_PREFIX(name) prefix_ ## name

// Generate a switch statement for handling multiple cases
#define SWITCH_CASE_GENERATOR(x) \
    case x: \
        printf("Case %d\n", x); \
        break;

int main()
{
    int num = 5;
    printf("Square of %d is %d\n", num, SQUARE(num));

    #ifdef FEATURE_A
        printf("Feature A is enabled.\n");
    #else
        printf("Feature A is disabled.\n");
    #endif

    int CONCAT(num, 1) = 10; // Creates a variable named num1
    printf("Value of num1: %d\n", num1);

    int ADD_PREFIX(value) = 20; // Creates a variable named prefix_value
    printf("Value of prefix_value: %d\n", prefix_value);

    int val = 2;

    switch (val) {
        SWITCH_CASE_GENERATOR(1)
        SWITCH_CASE_GENERATOR(2)
        SWITCH_CASE_GENERATOR(3)
        default:
            printf("Default case\n");
            break;
    }

    return 0;
}