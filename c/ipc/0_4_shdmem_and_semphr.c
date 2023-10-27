/**
 * Using shared memory and binary semaphore with each other
 * Shared memory does not have any inherent synchronization method which is now provided by semaphore
 * 
 * System V Semaphores needs to be delinked through ipcrm -s <semaphore id>
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>  //System V Semaphore
#include <sys/wait.h>

// Define the buffer size and the key for shared memory and semaphores
#define BUFFER_SIZE 5
#define SHARED_MEMORY_KEY 1234
#define SEMAPHORE_KEY 5678

// Structure for the shared circular buffer
struct CircularBuffer 
{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
};

// Semaphore operations
struct sembuf sem_wait = {0, -1, 0}; // Decrement semaphore
struct sembuf sem_signal = {0, 1, 0}; // Increment semaphore

int main() 
{
    // Create or get the shared memory segment
    int shmid = shmget(SHARED_MEMORY_KEY, sizeof(struct CircularBuffer), IPC_CREAT | 0666);
    if (shmid == -1) 
    {
        perror("Shared memory creation failed");
        exit(1);
    }

    // Attach the shared memory segment
    struct CircularBuffer* buffer = shmat(shmid, NULL, 0);
    if (buffer == (struct CircularBuffer*) -1) 
    {
        perror("Shared memory attachment failed");
        exit(1);
    }

    // Create or get the semaphore
    int semid = semget(SEMAPHORE_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) 
    {
        perror("Semaphore creation failed");
        exit(1);
    }

    // Initialize semaphore value to 1 (binary semaphore)
    if (semctl(semid, 0, SETVAL, 1) == -1) 
    {
        perror("Semaphore initialization failed");
        exit(1);
    }

    // Producer process
    if (fork() == 0) 
    {
        int item = 1;
        while (1) 
        {
            // Wait for the semaphore
            semop(semid, &sem_wait, 1);

            // Produce an item and add it to the buffer
            buffer->buffer[buffer->in] = item;
            buffer->in = (buffer->in + 1) % BUFFER_SIZE;
            printf("Produced: %d\n", item);

            item++;

            // Signal the semaphore
            semop(semid, &sem_signal, 1);

            usleep(500000); // Sleep for a while
        }
    }

    // Consumer process
    if (fork() == 0) 
    {
        while (1) 
        {
            // Wait for the semaphore
            semop(semid, &sem_wait, 1);

            // Consume an item from the buffer
            int item = buffer->buffer[buffer->out];
            buffer->out = (buffer->out + 1) % BUFFER_SIZE;
            printf("Consumed: %d\n", item);

            // Signal the semaphore
            semop(semid, &sem_signal, 1);

            usleep(700000); // Sleep for a while
        }
    }

    // Parent process waits for children
    wait(NULL);
    wait(NULL);

    // Detach and remove shared memory and semaphore
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
