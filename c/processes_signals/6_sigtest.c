#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

// Signal handler function for SIGUSR1
//Raise kill signal from terminal itself - from other terminal $ kill -10 <pid>
void usr1_handler(int sig_num) {
    printf("Process (%d) got the SIGUSR1\n", getpid());
}

int main() {
    pid_t ret;
    int status;
    int role = -1;

    // Register the SIGUSR1 signal handler
    signal(SIGUSR1, usr1_handler);

    // Create a child process
    ret = fork();

    if (ret > 0) { // Parent Context
        printf("Parent: This is the parent process (pid %d)\n", getpid());
        role = 0;
        pause(); // Wait for a signal
        printf("Parent: Awaiting child exit\n");
        ret = wait(&status); // Wait for child to exit
    } else if (ret == 0) { // Child Context
        printf("Child: This is the child process (pid %d)\n", getpid());
        role = 1;
        pause(); // Wait for a signal
    } else { // Parent Context — Error
        printf("Parent: Error trying to fork() (%d)\n", errno);
    }

    printf("%s: Exiting...\n", ((role == 0) ? "Parent" : "Child"));

    return 0;
}
