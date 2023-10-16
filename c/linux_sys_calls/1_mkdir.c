#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    char *directories[] = {"a", "b", "c", "d", "e"};
    char *parentDirectory = "/workspaces/docker_c_cpp/c/linux_sys_calls/"; // Change this to the actual path
    int i;

    for (i = 0; i < 5; i++)
    {
        char path[256];
        snprintf(path, sizeof(path), "%s/%s", parentDirectory, directories[i]);

        // Create the directory with read, write, and execute permissions for the owner
        if (mkdir(path, S_IRWXU) == 0)
        {
            printf("Created directory: %s\n", path);
        }
        else
        {
            perror("Error creating directory");
            exit(1);
        }

        
        
    }

    sleep(10);
    // Remove the directories
    //  Delete subdirectories
    for (i = 0; i < 5; i++)
    {
        char subdirPath[256];
        snprintf(subdirPath, sizeof(subdirPath), "%s/%s", parentDirectory, directories[i]);

        if (rmdir(subdirPath) == 0)
        {
            printf("Deleted directory: %s\n", subdirPath);
        }
        else
        {
            perror("Failed to delete directory");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}