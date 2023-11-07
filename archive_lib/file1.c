#include <stdio.h>

int func1()
{
    printf("This is from function 1\n");

    return 0;
}

unsigned long min( unsigned long num1, unsigned long num2)
{
    // using the conditional operator to calcualte the min
    return (num1 > num2) ? num2 : num1;
}