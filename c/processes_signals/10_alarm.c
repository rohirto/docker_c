/**
 * an example of alarm to kill the current process if the user isn’t able to
enter a password in a reasonable amount of time 
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void wakeup( int sig_num )
{
    raise(SIGKILL);
}

#define MAX_BUFFER 80

int main()
{
    char buffer[MAX_BUFFER+1];
    int ret;

    // Register the wakeup function to handle the SIGALRM signal
    signal( SIGALRM, wakeup );

    // Prompt the user with a time limit
    printf("You have 3 seconds to enter the password\n");

    // Set an alarm for 3 seconds
    alarm(3);

    // Read input from the user
    ret = read( 0, buffer, MAX_BUFFER );

    // Cancel the alarm
    alarm(0);

    if (ret == -1) {
        // Handle read error here if needed
    } else {
        // Remove the newline character from the input
        buffer[strlen(buffer)-1] = 0;
        printf("User entered %s\n", buffer);
    }

    // The program exits without explicitly returning a value
}
