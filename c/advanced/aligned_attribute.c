/**
 * @file aligned_attribute.c
 * @author rohirto
 * @brief Aligned Attribute Explanation
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * The aligned attribute in GCC and compatible compilers is used to specify or increase the alignment of variables, 
 * structure members, or entire structures.
 * 
 * It can be used to:
 * - Increase alignment for performance optimization (e.g., cache alignment).
 * - Decrease alignment to pack data tightly (when combined with packed).
 * 
 */

#include <stdio.h>


int x __attribute__((aligned(16))); // Align to 16 bytes


/**
 * @brief The int b will be aligned to 8 bytes, possibly introducing padding between a and b (size 16).
 * char a - 1 x x x x x x x 
 * int  b - 1 1 1 1 1 1 1 1 
 * 
 */
struct MyStruct1 {
    char a;
    int b __attribute__((aligned(8))); // Align to 8 bytes
};

/**
 * @brief The structure itself will be aligned to a 32-byte boundary in memory (size 32).
 * char a - 1 x x x x x x x x x x x x x x x 
 * int  b - 1 1 1 1 x x x x x x x x x x x x 
 * 
 */
struct __attribute__((aligned(32))) MyStruct2 {
    char a;
    int b;
};

/**
 * @brief This ensures the structure is packed tightly (removing default padding) but still aligned to a 4-byte boundary.
 * char a - 1 x x x
 * int  b - 1 1 1 1
 * 
 */
struct __attribute__((packed, aligned(4))) MyStruct3 {
    char a;
    int b;
};

int main() {
    
    printf("Address of x: %p\n", &x);
    printf("Size of MyStruct: %lu\n", sizeof(struct MyStruct1));
    printf("Size of MyStruct: %lu\n", sizeof(struct MyStruct2));
    printf("Size of MyStruct: %lu\n", sizeof(struct MyStruct3));
    return 0;
}