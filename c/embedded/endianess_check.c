#include <stdio.h>

int main() {
    unsigned int value = 0x01020304;
    unsigned char *bytePtr = (unsigned char *)&value;

    if (*bytePtr == 0x04) {
        printf("Little Endian\n");
    } else if (*bytePtr == 0x01) {
        printf("Big Endian\n");
    } else {
        printf("Unknown Endianness\n");
    }

    return 0;
}