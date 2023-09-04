#include <stdio.h>

int main() {
    int n;

    printf("Enter the number of rows for the pyramid: ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        // Print spaces
        for (int j = 1; j <= n - i; j++) {
            printf(" ");
        }

        // Print asterisks
        for (int j = 1; j <= 2 * i - 1; j++) {
            printf("*");
        }

        printf("\n");
    }

    return 0;
}