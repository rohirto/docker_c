#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void usr1_handler(int sig_num) {
    printf("Parent (%d) got the SIGUSR1\n", getpid());
}

int main() {
    pid_t ret;
    int status;
    int role = -1;

    ret = fork();

    if (ret > 0) { // Parent Context
        printf("Parent: This is the parent process (pid %d)\n", getpid());
        
        // Register a signal handler for SIGUSR1
        signal(SIGUSR1, usr1_handler);
        
        role = 0;

        // Wait for a signal to resume
        pause();

        printf("Parent: Awaiting child exit\n");
        
        // Wait for the child to exit
        ret = wait(&status);
    } else if (ret == 0) { // Child Context
        printf("Child: This is the child process (pid %d)\n", getpid());
        
        role = 1;

        // Simulate some work
        sleep(1);

        printf("Child: Sending SIGUSR1 to pid %d\n", getppid());
        
        // Send SIGUSR1 signal to the parent
        kill(getppid(), SIGUSR1);
        
        // Simulate more work
        sleep(2);
    } else { // Parent Context — Error
        printf("Parent: Error trying to fork() (%d)\n", errno);
    }

    printf("%s: Exiting...\n", ((role == 0) ? "Parent" : "Child"));

    return 0;
}
