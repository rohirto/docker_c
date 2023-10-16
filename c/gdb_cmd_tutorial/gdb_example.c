#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 5;
    int y = 7;
    int result;

    printf("Hello, GDB!\n");

    result = add(x, y);

    printf("Result: %d\n", result);

    return 0;
}
