#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid, wpid;
    int status;

    // Create a child process
    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0) {
        // This code is executed by the child process
        printf("Child process (PID %d) is running.\n", getpid());
        sleep(2); // Simulate some work in the child
        exit(42); // Exit with status 42
    } else {
        // This code is executed by the parent process
        printf("Parent process (PID %d) is waiting for the child to finish.\n", getpid());

        // Wait for the child to exit and store its exit status
        wpid = wait(&status);

        if (wpid == -1) {
            perror("Wait error");
            exit(1);
        }
        //This macros are used to check how the child process exited
        if (WIFEXITED(status)) {
            // Child exited normally
            printf("Child (PID %d) exited normally with status %d.\n", wpid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            // Child exited due to a signal
            printf("Child (PID %d) exited due to signal %d.\n", wpid, WTERMSIG(status));
        }
    }

    return 0;
}
