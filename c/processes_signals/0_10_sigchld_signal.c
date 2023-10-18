/**
 * A efficent way to close zombie processes using wait in a nonblocking mode.
 * The Linux system sends SIGCHLD Signal to parent process which is handled here 
 * wait will clean up the terminated child process and ensure that no zombie processes are there
*/
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

sig_atomic_t child_exit_status;

void clean_up_child_process(int signal_number)
{
    /* Clean up the child process. */
    int status;
    wait(&status);
    
    /* Store its exit status in a global variable. */
    child_exit_status = status;
    printf("Cleaned up Child process");
}

int main()
{
    pid_t child_pid;

    /* Handle SIGCHLD by calling clean_up_child_process. */
    struct sigaction sigchld_action;
    memset(&sigchld_action, 0, sizeof(sigchld_action));
    
    sigchld_action.sa_handler = &clean_up_child_process;
    sigaction(SIGCHLD, &sigchld_action, NULL);

    /* Now do things, including forking a child process. */
    /* ... */
    /* Create a child process. */
    child_pid = fork();

    if (child_pid > 0) 
    {
        //Parent Process
        for(int i = 0; i < 10; i++)
        {
            printf("Parent Process \n");
            sleep(1);
        }
    }
    else if (child_pid == 0)
    {
        // Child Process
        for(int i = 0; i < 5; i++)
        {
            printf("Child Process \n");
            sleep(1);
        }

    }
    else
    {
        // Error
        // An error occurred while forking

        printf("Parent: Error trying to fork() (%d)\n", errno);
    }


    return 0;
}
