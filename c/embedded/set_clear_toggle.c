#include <stdio.h>

// Function to set a specific bit in a number
int setBit(int num, int pos) {
    return num | (1 << pos);
}

// Function to clear a specific bit in a number
int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}

// Function to toggle a specific bit in a number
int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}

int main() {
    int num;
    printf("Enter an integer: ");
    scanf("%d", &num);

    int bitPos;
    printf("Enter the bit position (0-31): ");
    scanf("%d", &bitPos);

    int choice;
    printf("Choose operation:\n");
    printf("1. Set Bit\n");
    printf("2. Clear Bit\n");
    printf("3. Toggle Bit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            num = setBit(num, bitPos);
            break;
        case 2:
            num = clearBit(num, bitPos);
            break;
        case 3:
            num = toggleBit(num, bitPos);
            break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }

    printf("Modified number: %d\n", num);

    return 0;
}