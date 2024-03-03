/**
 * @file pragma.c
 * @author rohirto
 * @brief demonstrate capabilities of pragma
 * @version 0.1
 * @date 2023-11-28
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * #pragma directives in C are compiler-specific instructions that can be used to control various aspects of compilation 
 * and provide hints or optimizations to the compiler. Here are a few examples demonstrating the use of #pragma:
 * 
 * 
 * Keep in mind that the use of #pragma directives can make your code less portable across different compilers. 
 * It's essential to be aware of the specific directives supported by the compiler you are using. The examples provided here 
 * are tailored for the GCC compiler, and other compilers may have different or additional directives.
 */

#include <stdio.h>

/**
 * @brief #pragma GCC optimize("O3") directive suggests aggressive optimization (-O3 level) to the GCC compiler. 
 * The compiler may use this information to apply more aggressive optimization strategies to the code.
 * 
 */
#pragma GCC optimize("O3")


/**
 * @brief The #pragma GCC optimize("unroll-loops") directive hints the GCC compiler to unroll loops. Loop unrolling is an 
 * optimization technique where the compiler generates multiple copies of loop bodies to reduce loop overhead.
 * 
 */
#pragma GCC optimize("unroll-loops")


/**
 * @brief The #pragma GCC diagnostic directives can be used to control compiler diagnostics. In this example, 
 * it temporarily suppresses warnings for unused variables within a specific scope.
 * 
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"


/**
 * @brief The #pragma pack(push, 1) and #pragma pack(pop) directives control the alignment of structure members. 
 * In this example, it ensures that the struct has a packing of 1 byte.
 * 
 */
#pragma pack(push, 1) //Such practice can be used to pack the data to send over sockets or serial channel

struct Example {
    char c;
    int i;
};

#pragma pack(pop)


int main() {
    // Your code here
    printf("Optimized code example.\n");

    // Your code with loops
    printf("Code with loop unrolling example.\n");

    int unusedVariable;  // This will not trigger a warning
    struct Example e;
    printf("Unused variable example.\n");

    #pragma GCC diagnostic pop

    e.c = 'c';
    e.i = 128;

    printf("Size of c and i in struct: %zu %zu\n", sizeof(e.c), sizeof(e.i));

    
    return 0;
}