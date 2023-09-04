#include <stdio.h>

// Function to swap two bits at given positions in a byte
unsigned char swapBits(unsigned char num, int bitPos1, int bitPos2) {
    // Extract the bits at the given positions
    unsigned char bit1 = (num >> bitPos1) & 1;
    unsigned char bit2 = (num >> bitPos2) & 1;

    // XOR operation to swap the bits
    num ^= (bit1 << bitPos2) | (bit2 << bitPos1);

    return num;
}

int main() {
    unsigned char byte = 0b11010101; // Example input byte
    int bitPos1 = 2; // First bit position to swap
    int bitPos2 = 5; // Second bit position to swap

    printf("Original byte: 0x%02X\n", byte);
    
    // Call the swapBits function
    byte = swapBits(byte, bitPos1, bitPos2);

    printf("Modified byte: 0x%02X\n", byte);

    return 0;
}