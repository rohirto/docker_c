/*C program to demonstrate example of Variable Arguments.*/

#include <stdio.h>
#include <stdarg.h>

/*find sum of numbers*/
int sum(int N, ...)
{
    int loop, sum;
    va_list va; /*for argument list*/

    va_start(va, N); /*init with number of arguments*/

    /*access arguments & calculating sum*/
    sum = 0;
    for (loop = 0; loop < N; loop++) {
        sum += va_arg(va, int);
    }
    va_end(va);
    return sum;
}

int main()
{
    printf("Sum of 10, 20 = %d\n", sum(2, 10, 20));
    printf("Sum of 10, 20, 30, 40 = %d\n", sum(4, 10, 20, 30, 30));
    printf("Sum of 10, 20, 30, 40, 50, 60 = %d\n", sum(6, 10, 20, 30, 30, 40, 50));

    return 0;
}