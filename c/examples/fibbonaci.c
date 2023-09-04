#include <stdio.h>

void generateFibonacci(int n) {
    unsigned long first = 0, second = 1, next;

    printf("Fibonacci Series up to %d terms:\n", n);

    for (int i = 0; i < n; i++) {
        if (i <= 1) {
            next = i;
        } else {
            next = first + second;
            first = second;
            second = next;
        }
        printf("%lu ", next);
    }
    printf("\n");
}

int main() {
    int terms;
    printf("Enter the number of terms in Fibonacci Series: ");
    scanf("%d", &terms);

    if (terms <= 0) {
        printf("Please enter a positive number of terms.\n");
    } else {
        generateFibonacci(terms);
    }

    return 0;
}