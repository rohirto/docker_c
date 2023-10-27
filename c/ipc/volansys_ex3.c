/**
 * Develop a simple C based shell, which continuously scans for user input and accordingly execute commands. This shell should validate user inputs.
 *  Extra: How would you extend this shell, so it provides “|” (Linux pipe) feature? (10 points)
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
#define MAX_ARGS 10



void chck_exit(char *cmd)
{
    if (strcmp(cmd, "exit") == 0)
    {
        printf("Goodbye!\n");
        exit(0);
    }
}

int check_pipes(char* cmd, char* command_list[])
{
    
    // Split the command string into arguments using whitespace as the delimiter
    char* token = strtok(cmd, "|");
    int cmdCount = 0;

    while (token != NULL) {
        command_list[cmdCount] = token;
        cmdCount++;
        token = strtok(NULL, "|");
    }

    // Add a NULL pointer at the end to indicate the end of the argument list
    command_list[cmdCount] = NULL;

    // Print the parsed arguments
    for (int i = 0; i < cmdCount; i++) {
        printf("command %d: %s\n", i, command_list[i]);
    }

    return cmdCount;
}



int check_arg(char* cmd, char* argument_list[])
{
    
    // Split the command string into arguments using whitespace as the delimiter
    char* token = strtok(cmd, " ");
    int argCount = 0;

    while (token != NULL) {
        argument_list[argCount] = token;
        argCount++;
        token = strtok(NULL, " ");
    }

    // Add a NULL pointer at the end to indicate the end of the argument list
    argument_list[argCount] = NULL;

    // Print the parsed arguments
    for (int i = 0; i < argCount; i++) {
        printf("Arg %d: %s\n", i, argument_list[i]);
    }

    //add NULL at the end
    argument_list[++argCount] = NULL;
}

void exec_command(char* command_list[], int cmdCount)
{
    pid_t childpid;
    int status;
    char* args[MAX_ARGS];
    

    //Check arguments 
    for (int i =0; i < cmdCount; i++)
    {
        check_arg(command_list[i],args);
        int fds[2];
        /* Create a pipe. File descriptors for the two ends of the pipe are placed in fds. */
        pipe(fds);
        //Now wer have all what we need in args
        childpid = fork(); // Create a child process

        if (childpid == 0) 
        {
            if (i > 0) 
            {
                // Redirect input from the previous pipe
                dup2(fds[0], 0);
                close(fds[0]);
            }

            if (i < cmdCount - 1) // No of Pipes 
            {
                /* Connect the write end of the pipe to standard input. */
                dup2(fds[1], STDOUT_FILENO);
                close(fds[1]);
            }

            /* This is the child process. Close our copy of the write end of the file descriptor. */
            close(fds[1]);
            // Child process
            execvp(args[0], args); // Execute the command

        } 
        else if (childpid > 0) 
        {
            // Parent process
            waitpid(childpid, &status, 0); // Wait for the child to finish

            for (i = 0; i < cmdCount -1; i++)
            {
                close(fds[0]);
                close(fds[1]);
            }
            
        }

    }
    
}

int main()
{
    int status;
    pid_t childpid;
    char cmd[MAX_LINE + 1];
    char *sret;
    
    char* piped_cmds[10];
    int no_of_cmd;

    while (1) 
    {
        printf("mysh>"); // Print the shell prompt

        if (fgets(cmd, sizeof(cmd), stdin) == NULL) 
        {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove the trailing newline character
        cmd[strcspn(cmd, "\n")] = '\0';

        //Check Exit 
        chck_exit(cmd);

        //Check Pipes
        no_of_cmd = check_pipes(cmd,piped_cmds);

        //execute the commands
        exec_command(piped_cmds, no_of_cmd);

        //Check arguments
        //check_arg(cmd,args);
        



        // childpid = fork(); // Create a child process

        // if (childpid == 0) {
        //     // Child process
        //     execlp(cmd, cmd, NULL); // Execute the command
        // } else if (childpid > 0) {
        //     // Parent process
        //     waitpid(childpid, &status, 0); // Wait for the child to finish
        // }

        // printf("\n"); // Print a new line
    }

    return 0;
}
