#include <stdio.h>
int main()
{

    unsigned char a=0x00;   
    a= a|0x01;  //01
    a= a|0x02;  //02
    a= a|0x04;  //07
    a= a|0x08;  //0F
    printf("a= %02X\n",a);                                       

    return 0;

}