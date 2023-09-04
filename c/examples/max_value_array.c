#include <stdio.h>

int findMaxValue(const int arr[], int size) {
    int max = arr[0]; // Assume the first element is the maximum

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i]; // Update max if a larger value is found
        }
    }

    return max;
}

int main() {
    int size;

    printf("Enter the size of the array: ");
    scanf("%d", &size);

    int arr[size];

    printf("Enter %d integers:\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    int maxValue = findMaxValue(arr, size);

    printf("The maximum value in the array is: %d\n", maxValue);

    return 0;
}