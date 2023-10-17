/**
 * Assert is used a check condition to catch the bugs at early stage,
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main() {
    int x = 10;
    int y = 20;
    int arr[5] = {1, 2, 3, 4, 5};

    // Example 1: Check if an expression is true
    assert(x < y);

    // Example 2: Check if a pointer is not NULL
    int *ptr = NULL;
    #if 1
    ptr = (int*)malloc(sizeof(int));
    *ptr = x;
    #endif
    assert(ptr != NULL);
    #if 1
    free(ptr);
    #endif

    // Example 3: Check if an array index is within bounds
    int index = 4;
    assert(index >= 0 && index < 5);

    // Example 4: Check for division by zero
    #if 0
    int divisor = 0;
    #else
    int divisor = 1;
    #endif
    assert(divisor != 0);

    // Example 5: Custom assertion message
    #if 0
    int value = -1;
    #else
    int value = 1;
    #endif
    assert(value >= 0 && "Value must be non-negative");

    printf("All assert conditions passed!\n");

    return 0;
}