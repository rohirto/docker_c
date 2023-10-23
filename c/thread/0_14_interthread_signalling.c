/**
 * Inter Thread signalling in Linux.
 * As threads are just processes with dff implementation (clone sys call of Linux)
*/
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signo) 
{
    printf("Thread received signal: %d\n", signo);
}

void* thread_function(void* arg) 
{
    // Register the signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigaction(SIGUSR1, &sa, NULL);

    while (1) 
    {
        printf("Thread is running...\n");
        sleep(1);
    }

    return NULL;
}

int main() 
{
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);

    // Sleep briefly to allow the thread to start
    sleep(2);

    // Send a signal to the thread
    pthread_kill(thread, SIGUSR1);

    // Sleep to keep the program running
    sleep(5);

    return 0;
}
