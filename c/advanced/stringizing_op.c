/*
Stringizing Operator: '#' in preprocessor directive is known as Stringizing Operator it is used
 to convert an argument into string format.
*/

#include <stdio.h>

#define getVariableName(x) #x
#define printDebug(x) printf("\nvalue of \"%s\" is: %d\n", #x, x);

int main()
{
    int student_age = 21;
    int value1;
    int value2;

    value1 = 10;
    value2 = 20;

    printf("value of %s is = %d\n", getVariableName(student_age), student_age);

    printDebug(value1);
    printDebug(value2);

    return 0;
}