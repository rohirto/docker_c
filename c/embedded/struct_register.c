#include <stdio.h>
#include <stdint.h>

// Define a structure for the configuration register
struct ConfigurationRegister {
    uint16_t reserved : 8;        // 8-bit reserved field
    uint16_t prescaler : 4;       // 4-bit prescaler value
    uint16_t mode : 3;            // 3-bit mode selection
    uint16_t enable_disable : 1;  // 1-bit enable/disable
};

// Function to print a 16-bit value in binary format
void printBinary(uint16_t value, int numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

int main() {
    // Create an instance of the ConfigurationRegister structure
    struct ConfigurationRegister config;

    // Initialize the configuration register
    config.reserved = 0;
    config.prescaler = 7;
    config.mode = 2;
    config.enable_disable = 1;

    // Print the binary representation of each field
    printf("Reserved:        ");
    printBinary(config.reserved, 8);
    printf("\n");

    printf("Prescaler:       ");
    printBinary(config.prescaler, 4);
    printf("\n");

    printf("Mode Selection:  ");
    printBinary(config.mode, 3);
    printf("\n");

    printf("Enable/Disable:  ");
    printBinary(config.enable_disable, 1);
    printf("\n");

    return 0;
}