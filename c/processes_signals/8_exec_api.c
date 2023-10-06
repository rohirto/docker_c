/**
 * The exec family of functions replaces the current process image altogether.
 * The exec function starts a new program, replacing the current process, while retaining the current pid
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char *argv[] = {"ls", "-l", NULL};
    char *envp[] = {"VAR=value", NULL};
    
    /**
     * execvp: This function searches for the program in the directories listed in the PATH environment variable. 
     * You provide the program name and its arguments as separate arguments to execvp. For example:
    */
    printf("Executing execvp:\n");
    execvp("ls", argv);
    perror("execvp"); // This line will only execute if execvp fails

    /**
     * execv: Similar to execvp, but you need to provide the full path to the program as the first argument.
    */
    printf("Executing execv:\n");
    execv("/bin/ls", argv);
    perror("execv");

    /**
     * execlp: This function is used when you want to specify the program and its arguments as separate strings in the function call.
    */
    printf("Executing execlp:\n");
    execlp("ls", "ls", "-l", NULL);
    perror("execlp");

    /***
     * execle: Similar to execlp, but it also allows you to pass a custom environment to the new program. You provide the program name,
     *  its arguments, and the environment as separate arguments.
    */
    printf("Executing execle:\n");
    execle("/bin/ls", "ls", "-l", NULL, envp);
    perror("execle");

    /** 
     * execvpe: Like execvp, but you can specify a custom environment.
    */
    printf("Executing execvpe:\n");
    execvpe("ls", argv, envp);
    perror("execvpe");

    /**
     * execve: Allows you to specify the program, its arguments, and the environment separately.
     *  You provide the full path to the program.
    */
    printf("Executing execve:\n");
    execve("/bin/ls", argv, envp);
    perror("execve");

    return 0;
}
