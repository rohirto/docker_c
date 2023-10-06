#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid;
    int status;

    // Create a child process
    child_pid = fork();

    if (child_pid == 0) {
        // This code is executed by the child process
        printf("Child process (PID %d) is running\n", getpid());

        // Simulate some work in the child
        sleep(2);

        // Exit the child with a status code
        int exit_status = 42;
        exit(exit_status);
    } else if (child_pid > 0) {
        // This code is executed by the parent process
        printf("Parent process (PID %d) is waiting for the child to exit...\n", getpid());

        // Wait for the child to exit and get its status
        wait(&status);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Child process (PID %d) exited with status code: %d\n", child_pid, exit_status);
        }
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}
