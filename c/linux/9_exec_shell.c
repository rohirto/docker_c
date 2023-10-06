#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80

int main()
{
    int status;
    pid_t childpid;
    char cmd[MAX_LINE + 1];
    char *sret;

    while (1) {
        printf("mysh>"); // Print the shell prompt

        sret = fgets(cmd, sizeof(cmd), stdin); // Read user input

        if (sret == NULL)
            exit(-1); // Exit if there's an error reading input

        cmd[strlen(cmd) - 1] = 0; // Remove the newline character

        if (!strncmp(cmd, "bye", 3))
            exit(0); // Exit if the user enters "bye"

        childpid = fork(); // Create a child process

        if (childpid == 0) {
            // Child process
            execlp(cmd, cmd, NULL); // Execute the command
        } else if (childpid > 0) {
            // Parent process
            waitpid(childpid, &status, 0); // Wait for the child to finish
        }

        printf("\n"); // Print a new line
    }

    return 0;
}
