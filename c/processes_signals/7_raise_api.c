#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//raise is a watered down version of kill

// Signal handler for SIGALRM
void alarm_handler(int signum) {
    printf("Received an alarm signal (SIGALRM)\n");
}

int main() {
    // Register the signal handler for SIGALRM
    signal(SIGALRM, alarm_handler);

    printf("Process ID: %d\n", getpid());

    // Set an alarm to send a SIGALRM signal after 5 seconds
    alarm(5);

    // Sleep to allow the alarm to trigger
    sleep(10);

    // Demonstrate raise: Send a SIGINT signal to itself
    // We demonstrate the use of raise(SIGINT) to send a SIGINT signal to the current process, effectively terminating it.
    raise(SIGINT);

    // Demonstrate execlp: Replace the current process with 'ls' command
    execlp("ls", "ls", "-l", NULL);

    // This code is reached only if execlp fails
    perror("execlp");
    exit(EXIT_FAILURE);

    return 0;
}
