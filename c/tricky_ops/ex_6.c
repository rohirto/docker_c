#include <stdio.h>
int main()
{
    unsigned char a=0xAA;
    if(a & 0x01)                //false
        printf("one - true\n");
    else
        printf("one - false\n");                      
    if(a & 0x02)                // true
        printf("two - true\n");
    else
        printf("two - false\n");                                       

    return 0;

}