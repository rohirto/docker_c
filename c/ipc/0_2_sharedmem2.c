/**
 * Example of using Shared memory as a IPC 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHARED_MEM_KEY 12345
#define SHARED_MEM_SIZE 1024

int main() 
{
    int shmid;
    char *shared_memory;

    // Create or access the shared memory segment.
    shmid = shmget(SHARED_MEM_KEY, SHARED_MEM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) 
    {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment to this process.
    shared_memory = (char *)shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1) 
    {
        perror("shmat");
        exit(1);
    }

    // Parent process writes to shared memory.
    if (fork() > 0) 
    {
        printf("Parent process writing to shared memory...\n");
        strcpy(shared_memory, "Hello from the parent process!");
        sleep(2);  // Sleep to allow the child process to read.

        // Detach and remove shared memory.
        shmdt(shared_memory);
        shmctl(shmid, IPC_RMID, NULL);
    }
    // Child process reads from shared memory.
    else 
    {
        sleep(1);  // Sleep to allow the parent process to write.
        printf("Child process reading from shared memory: %s\n", shared_memory);

        // Detach shared memory.
        shmdt(shared_memory);
    }

    return 0;
}
