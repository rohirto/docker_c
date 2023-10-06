#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

//Callback Function called after signal is caught
void catch_ctlc( int sig_num )
{
    printf( "Caught Control-C\n" );
    fflush( stdout );

    return;
}

int main()
{
    //Register the Callback
    signal(SIGINT, catch_ctlc); 

    printf("Go ahead, make my day.\n");

    //Suspend the process until a signal arrives
    pause();

    return 0;
}