/**
 * @file packed_attribute.c
 * @author rohirto
 * @brief Packed Attribute in C
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * The __attribute__((packed)) variable attribute in GCC and other compatible compilers specifies that a variable or structure field 
 * should have the smallest possible alignment, minimizing memory padding between its members. 
 * 
 *  It is typically used in scenarios where memory layout is crucial, such as when working with hardware registers, network protocols, or file formats.
 * 
 * __attribute__((packed)) eliminates this padding, forcing members to be stored in contiguous memory locations.
 * 
 */

#include <stdio.h>
#include <stdint.h>


/**
 * @brief This is a unpacked struct thus padded (x denotes padding)
 * char     a - 1 x x x
 * int      b - 1 1 1 1
 * short    c - 1 1 x x
 * 
 * Thus total struct size 12 bytes
 * 
 */
struct MyStructUnPacked {
    char a;
    int b;
    short c;
};


/**
 * @brief This is a packed struct thus non padded
 * char     a - 1 
 * int      b - 1 1 1 1
 * short    c - 1 1 
 * 
 * Thus total struct size 7 bytes
 * 
 */
struct __attribute__((packed)) MyStructPacked {
    char a;
    int b;
    short c;
};


/**
 * @brief Use case of packed struct - Hardware reg - where memory alignment is important
 * 
 */
struct __attribute__((packed)) HardwareRegister {
    uint8_t control;
    uint16_t status;
    uint8_t command;
};

/**
 * @brief Use case of Packed struct - Protocol Headers
 * 
 * @return int 
 */
struct __attribute__((packed)) IPHeader {
    uint8_t version_and_ihl;
    uint8_t type_of_service;
    uint16_t total_length;
};

int main() {
    printf("Size of MyStructUnPacked: %lu\n", sizeof(struct MyStructUnPacked));
    printf("Size of MyStructPacked: %lu\n", sizeof(struct MyStructPacked));
    return 0;
}