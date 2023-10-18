/**
 * Run this program in one terminal
 * From other terminal - $ kill -s SIGUSR1 <pid of this process>
 * Then the code flow is interrupted bu USER Signal
*/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//This ensures that our global variable is atomic, safe fro other threads and processes
sig_atomic_t sigusr1_count = 0;

// Signal handler function
void handler(int signal_number)
{
    ++sigusr1_count;
    // Print the number of times SIGUSR1 was raised
    printf("SIGUSR1 was raised %d times\n", sigusr1_count);
}

int main() 
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    // Set the signal handler to the 'handler' function for SIGUSR1
    sa.sa_handler = &handler;
    
    // Register the signal handler with the system for SIGUSR1
    sigaction(SIGUSR1, &sa, NULL);

    /* Do some lengthy stuff here. */
    /* ... */
    while(1)
    {

    }
    
    

    return 0;
}
