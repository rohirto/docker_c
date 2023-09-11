#include <stdio.h>
int main()
{
    unsigned int x = 10 ;
    int y = -40;
    if(x+y > 10)    //expression x + y will result in integer promotion, When you perform operations 
                    //with unsigned int and int, the int will be promoted to an unsigned int
                    //-40 (as a signed integer) will be converted to an unsigned int, which is effectively a large positive value.
    {   
        printf("Greater than 10");
    } 
    else 
    {
        printf("Less than or equals 10");
    }

    return 0;
}