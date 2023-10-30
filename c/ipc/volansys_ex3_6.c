#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main() 
{
    char input[MAX_INPUT_SIZE];

    while (1) 
    {
        // Display a prompt
        printf("mysh> ");

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) 
        {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove the trailing newline character
        input[strcspn(input, "\n")] = '\0';

        // Validate and execute the user command
        if (strlen(input) > 0) 
        {
            // You can add your validation logic here

            // Fork a child process to execute the command
            pid_t child_pid = fork();
            if (child_pid == 0) 
            {
                // Child process
                char *args[] = {
                    "/bin/sh",  // Use /bin/sh to run user commands
                    "-c",
                    input,      // User input as the command to execute
                    NULL
                };

                execvp("/bin/sh", args);
                perror("execvp");  // Exec failed
                exit(EXIT_FAILURE);
            } else if (child_pid > 0) 
            {
                // Parent process
                int status;
                waitpid(child_pid, &status, 0);
            } else 
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
