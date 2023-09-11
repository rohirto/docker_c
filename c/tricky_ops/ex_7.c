#include <stdio.h>
int main()
{
    unsigned char a=0xAA;
    unsigned char b=0x55;

    printf("(a^b): %02X\n",(a^b));     //0b10101010 & 0b01010101 -> 0b11111111            

    return 0;

}