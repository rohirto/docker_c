#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t ret;        // Variable to store the return value of fork
    int status, i;    // Variables for process status and loop counter
    int role = -1;    // Variable to determine if it's the parent or child

    // Create a new process by duplicating the current process
    ret = fork();

    if (ret > 0) {
        // This code is executed in the parent process

        printf("Parent: This is the parent process (pid %d)\n", getpid());

        // Loop 10 times, printing a message and sleeping for 1 second each time
        for (i = 0; i < 10; i++) {
            printf("Parent: At count %d\n", i);
            sleep(1);
        }

        // Wait for the child process to finish
        ret = wait(&status);

        // Set the role to 0 to indicate it's the parent
        role = 0;
    } else if (ret == 0) {
        // This code is executed in the child process

        printf("Child: This is the child process (pid %d)\n", getpid());

        // Loop 10 times, printing a message and sleeping for 1 second each time
        for (i = 0; i < 10; i++) {
            printf("Child: At count %d\n", i);
            sleep(1);
        }

        // Set the role to 1 to indicate it's the child
        role = 1;
    } else {
        // An error occurred while forking

        printf("Parent: Error trying to fork() (%d)\n", errno);
    }

    // Print a message indicating which process is exiting
    printf("%s: Exiting...\n", ((role == 0) ? "Parent" : "Child"));

    return 0;
}
