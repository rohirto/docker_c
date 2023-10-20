/**
 * Create 5 directories into existing directories using Linux System calls
*/
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char* current_path = "/workspaces/docker_c_cpp/c/linux_sys_calls/temp";  // The directory where you want to create the directories
    const char dir_names[5] = {'a','b','c','d','e'}; 
    for (int i = 0; i < 5; i++)
    {
        char newDir[256];

        // Create the path with dir name to be passed to system call
        snprintf(newDir, sizeof(newDir), "%s/%c", current_path, dir_names[i]);

        //Make the system call, pass the permissions to the folder aswell
        if (syscall(SYS_mkdir, newDir, 0777) == 0) 
        {
            printf("Created directory: %s\n", newDir);
        } else 
        {
            perror("Error creating directory");
            exit(1);
        }
    }

    return 0;
}

