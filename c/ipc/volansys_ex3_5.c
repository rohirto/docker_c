#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_LINE    100
#define MAX_ARGS    10
struct command
{
    char **argv;
};


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

int spawn_proc(int in, int out, struct command *cmd)
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        if (in != 0)
        {
            dup2(in, 0);
            close(in);
        }

        if (out != 1)
        {
            dup2(out, 1);
            close(out);
        }

        return execvp(cmd->argv[0], (char *const *)cmd->argv);
    }

    return pid;
}

int fork_pipes(int n, struct command *cmd)
{
    int i;
    pid_t pid;
    int in, fd[2];

    /* The first process should get its input from the original file descriptor 0. */
    in = 0;

    /* Note the loop bound, we spawn here all but the last stage of the pipeline. */
    for (i = 0; i < n - 1; ++i)
    {
        pipe(fd);

        /* fd[1] is the write end of the pipe, we carry `in` from the previous iteration. */
        spawn_proc(in, fd[1], cmd + i);

        /* No need for the write end of the pipe; the child will write here. */
        close(fd[1]);

        /* Keep the read end of the pipe; the next child will read from there. */
        in = fd[0];
    }

    /* Last stage of the pipeline - set stdin to be the read end of the previous pipe
       and output to the original file descriptor 1. */
    if (in != 0)
        dup2(in, 0);

    /* Execute the last stage with the current process. */
    return execvp(cmd[i].argv[0], (char *const *)cmd[i].argv);
}

int main()
{
    char cmd[MAX_LINE + 1];
    char* piped_cmds[10];
    int no_of_cmd, status;
    char* args[MAX_ARGS];
    pid_t childpid;
    
    while(1)
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

        if (strcmp(cmd, "\n") == 0)
        {
            continue;
        }

        //Check Pipes
        no_of_cmd = check_pipes(cmd,piped_cmds);

        //Check arguments 
        for (int i =0; i < no_of_cmd; i++)
        {
            check_arg(piped_cmds[i],&args[i]);
        }
        struct command cmds[no_of_cmd];
        for(int i = 0; i < no_of_cmd; i++)
        {
            cmds[i].argv = &args[i];
        }
        

        childpid = fork();

        if(childpid == 0)
        {
            //If child then exec command
            fork_pipes(no_of_cmd,cmds);

        }
        else
        {
            //Parent
            waitpid(childpid, &status, 0); // Wait for the child to finish
        }
        

    }
    // const char *ls[] = { "ls", "-l", NULL };
    // const char *awk[] = { "awk", "{print $1}", NULL };
    // const char *sort[] = { "sort", NULL };
    // const char *uniq[] = { "uniq", NULL };

    // struct command cmd_struct[] = { { ls }, { awk }, { sort }, { uniq } };

    // return fork_pipes(4, cmd);
    return 0;
}
