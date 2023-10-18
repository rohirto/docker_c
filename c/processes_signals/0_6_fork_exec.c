/**
 * Using Fork-Exec method to create new process instead of previous sytem function
 * Fork -> Creates a new child process, exec -> program calling exec ceases and new program passed into exec 
 *         executes
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


/* Spawn a child process running a new program. PROGRAM is the name
   of the program to run; the path will be searched for this program.
   ARG_LIST is a NULL-terminated list of character strings to be
   passed as the program’s argument list. Returns the process ID of
   the spawned process. */
int spawn(char* program, char** arg_list) {
    pid_t child_pid;

    /* Duplicate this process. */
    child_pid = fork();
    if (child_pid != 0) {
        /* This is the parent process. */
        return child_pid;
    } else {
        /* Now execute PROGRAM, searching for it in the path. */
        execvp(program, arg_list);

        /* The execvp function returns only if an error occurs. */
        fprintf(stderr, "An error occurred in execvp\n");
        abort();
    }
}

int main() {
    int child_status;
    /* The argument list to pass to the “ls” command. */
    char* arg_list[] = {
        "ls",   /* argv[0], the name of the program. */
        "-l",
        "/",
        NULL    /* The argument list must end with a NULL. */
    };

    /* Spawn a child process running the “ls” command. Ignore the
       returned child process ID. */
    spawn("ls", arg_list);
    printf("Done with the main program\n");

    /* Wait for the child process to complete. */
    wait(&child_status);

    if (WIFEXITED(child_status))
    {
        /* Child process exited normally. */
        printf("The child process exited normally, with exit code %d\n", WEXITSTATUS(child_status));
    }
    else
    {
        /* Child process exited abnormally. */
        printf("The child process exited abnormally\n");
    }
    return 0;
}
