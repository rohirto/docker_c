/**
 * The nice function in Linux is used to change the priority of a process. 
 * A higher nice value means lower priority, while a lower nice value means higher priority.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Get the current nice value
    int current_nice = nice(0);
    if (current_nice == -1) {
        perror("Failed to get current nice value");
        return 1;
    }

    printf("Current nice value: %d\n", current_nice);

    // Set a new nice value
    int new_nice = 10;  // You can set your desired nice value here
    int result = nice(new_nice);

    if (result == -1) {
        perror("Failed to set new nice value");
        return 1;
    }

    // Get and print the updated nice value
    int updated_nice = nice(0);
    if (updated_nice == -1) {
        perror("Failed to get updated nice value");
        return 1;
    }

    printf("Updated nice value: %d\n", updated_nice);

    return 0;
}
