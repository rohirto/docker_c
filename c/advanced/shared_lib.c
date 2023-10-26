/**
 * Shared Lib is a dynamically linked Library, executable merely contains a reference to shared library
 * This library is shared among all the programs that link with it
 * Use of -fPIC option to create a shared library (PIC - Position Independent Code)
 * $ gcc -c -fPIC test1.c
 * $ gcc -shared -fPIC -o libtest.so test1.o test2.o
 * $ gcc -o app app.o -L. –ltest
*/
// main.c
#include <stdio.h>
#include "mathfuncs.h"

int main() {
    extern int a_n;
    int a = 10, b = 5;
    printf("Addition: %d\n", add(a, b));
    printf("Subtraction: %d\n", subtract(a, b));
    printf("extern variable: %d", a_n);
    return 0;
}
