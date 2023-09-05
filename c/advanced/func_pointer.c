/**
 * A function pointer is a pointer variable that can store address of a function and then using the function 
 * pointer we can call initialized function in our program.
*/

#include <stdio.h>

//function: sum, will return sum of two
//integer numbers

int addTwoNumbers(int x, int y)
{
    return x + y;
}

int main()
{
    int a, b, sum;

    //function pointer declaration
    int (*ptr_sum)(int, int);
    //function initialisation
    ptr_sum = &addTwoNumbers;

    a = 10;
    b = 20;

    //function calling
    sum = (*ptr_sum)(a, b);

    printf("Sum is: %d\n", sum);

    return 0;
}