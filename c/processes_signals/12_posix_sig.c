#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

static int stopChild = 0;

void sigHandler(int sig, siginfo_t *siginfo, void *ignore)
{
    // This function is the signal handler for SIGUSR1.
    // It prints the process ID of the sender and sets the stopChild flag to 1.
    printf("Got SIGUSR1 from %d\n", siginfo->si_pid);
    stopChild = 1;
}

int main()
{
    pid_t ret;
    int status;
    int role = -1;

    // Create a child process.
    ret = fork();

    if (ret > 0) { // Parent process
        printf("Parent: This is the parent process (pid %d)\n", getpid());

        // Let the child initialize.
        sleep(1);

        // Send a SIGUSR1 signal to the child.
        kill(ret, SIGUSR1);

        // Wait for the child to exit.
        ret = wait(&status);
        role = 0;
    } else if (ret == 0) { // Child process
        struct sigaction act;

        printf("Child: This is the child process (pid %d)\n", getpid());

        // Set up a signal handler for SIGUSR1.
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction = sigHandler;
        sigaction(SIGUSR1, &act, 0);

        printf("Child Waiting...\n");

        // Wait until the stopChild flag is set by the signal handler.
        while (!stopChild);

        role = 1;
    } else { // Error in forking
        printf("Parent: Error trying to fork() (%d)\n", errno);
    }

    // Print whether it's the parent or child process and exit.
    printf("%s: Exiting...\n", (role == 0) ? "Parent" : "Child");
    return 0;
}
