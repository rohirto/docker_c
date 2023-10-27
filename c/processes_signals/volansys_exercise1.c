/**
 * Write a C program that communicates between child and parent processes using linux signals. [15 points]
 *  1.Create a child process from the parent process.
 *  2. Parent will send messages to the child using any different thress linux signals.
 *  3. Child needs to pick up these signals and calls appropriate functions for printing confirmation on screen. 

*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void signal_handler(int signo) 
{
    switch (signo) 
    {
        case SIGUSR1:
            printf("Child received SIGUSR1 signal from parent.\n");
            break;
        case SIGUSR2:
            printf("Child received SIGUSR2 signal from parent.\n");
            break;
        case SIGINT:
            printf("Child received SIGINT signal from parent. Exiting.\n");
            exit(0);
    }
}

int main() 
{
    pid_t child_pid;

    // Create a child process
    child_pid = fork();

    if (child_pid < 0) 
    {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) 
    { 
        // Child process
        signal(SIGUSR1, signal_handler);
        signal(SIGUSR2, signal_handler);
        signal(SIGINT, signal_handler);

        while (1) 
        {
            // Child process waits for signals from the parent
            sleep(1);
        }
    } 
    else 
    { 
        // Parent process
        printf("Parent process is sending signals to the child...\n");
        sleep(2);

        // Send SIGUSR1 to the child
        kill(child_pid, SIGUSR1);
        sleep(2);

        // Send SIGUSR2 to the child
        kill(child_pid, SIGUSR2);
        sleep(2);

        // Send SIGINT to the child to terminate it
        kill(child_pid, SIGINT);

        // Wait for the child to exit
        wait(NULL);
        printf("Parent process has completed.\n");
    }

    return 0;
}
