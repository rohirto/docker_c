#include <stdio.h>

// Function pointer type for a function that takes two int pointers and returns void
typedef void (*FunctionPointer)(int*,const int*);

// Example functions
void add(int* a, const int* b) {
    *a = *a + *b;
}

void subtract(int* a,const int* b) {
    *a = *a - *b;
}

int main() {
    int x = 10, y = 5;
    
    // Declare a function pointer variable
    FunctionPointer operation;

    // Use a function pointer to point to the add function
    operation = add;

    // Call the function through the pointer
    operation(&x, &y);
    printf("After addition: %d\n", x);

    // Change the function pointer to point to the subtract function
    operation = subtract;

    // Call the function through the pointer
    operation(&x, &y);
    printf("After subtraction: %d\n", x);

    return 0;
}