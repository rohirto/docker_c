#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char cmd[1024] ;
    char* command_list[3];
    char* arg_list[3];

    if (fgets(cmd, sizeof(cmd), stdin) == NULL)
    {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    // Step 1: Tokenize the input string by '|'
    char* tok = strtok(cmd, "|");
    int i = 0;
    while (tok != NULL) {
        command_list[i] = tok;
        i++;
        tok = strtok(NULL, "|");
    }

    int num_pipes = i - 1;
    int pipes[num_pipes][2];

    for (i = 0; i < num_pipes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    // Step 2: Parse and execute commands
    for (i = 0; i < num_pipes + 1; i++) {
        char* token = strtok(command_list[i], " ");
        int j = 0;
        while (token != NULL) {
            if (j == 0) {
                arg_list[i] = token; // The first token is the command
                token = strtok(NULL, " ");
            } else {
                arg_list[j] = token; // Subsequent tokens are arguments
                token = strtok(NULL, " ");
            }
            j++;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Child process
            if (i > 0) {
                // Redirect input from the previous pipe
                dup2(pipes[i - 1][0], 0);
                close(pipes[i - 1][0]);
            }

            if (i < num_pipes) {
                // Redirect output to the next pipe
                dup2(pipes[i][1], 1);
                close(pipes[i][1]);
            }

            close(pipes[i][0]); // Close read end of the current pipe
            execvp(arg_list[0], arg_list);
            perror("Execution failed");
            exit(1);
        }
    }

    // Close all pipe ends in the parent process
    for (i = 0; i < num_pipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to complete
    for (i = 0; i < num_pipes + 1; i++) {
        wait(NULL);
    }

    return 0;
}
