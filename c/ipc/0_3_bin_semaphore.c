/**
 * Implementation of a Process Semaphore (Binary Semaphore)
 * It is derivative of shared memory implementation
 * 
 * Allocation
 * Deallocation
 * Initialization
 * Wait
 * Post
 * 
 * Debugging Semaphores 
 *  Use the command ipcs -s to display information about existing semaphore sets
 * Use the ipcrm sem command to remove a semaphore set from the command line
*/
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* We must define union semun ourselves. */
union semun 
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Obtain a binary semaphore’s ID, allocating if necessary. */
int binary_semaphore_allocation(key_t key, int sem_flags)
{
    return semget(key, 1, sem_flags);
}

/* Deallocate a binary semaphore. All users must have finished their
   use. Returns -1 on failure. */
int binary_semaphore_deallocate(int semid)
{
    union semun ignored_argument;
    return semctl(semid, 0, IPC_RMID, ignored_argument);
}

/* Initialize a binary semaphore with a value of 1. */
int binary_semaphore_initialize(int semid)
{
    union semun argument;
    unsigned short values[1];
    values[0] = 1;
    argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}

/* Wait on a binary semaphore. Block until the semaphore value is positive, then
   decrement it by 1. */
int binary_semaphore_wait(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = -1;  // Decrement by 1
    operations[0].sem_flg = 0;
    return semop(semid, operations, 1);
}

/* Post to a binary semaphore: increment its value by 1. This returns
   immediately. */
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;  // Increment by 1
    operations[0].sem_flg = 0;
    return semop(semid, operations, 1);
}

int main() 
{
    key_t sem_key = ftok("keyfile", 1);  // Create a unique key for the semaphore.
    int semid = binary_semaphore_allocation(sem_key, IPC_CREAT | 0666); // Create a semaphore or open if it already exists

    if (semid < 0) 
    {
        perror("Semaphore allocation failed");
        exit(1);
    }

    // Initialize the semaphore with a value of 1.
    if (binary_semaphore_initialize(semid) < 0) 
    {
        perror("Semaphore initialization failed");
        exit(1);
    }

    printf("Semaphore created and initialized.\n");

    // Perform operations on the semaphore.
    printf("Waiting on the semaphore...\n");
    if (binary_semaphore_wait(semid) < 0) 
    {
        perror("Semaphore wait failed");
        exit(1);
    }

    printf("Semaphore acquired. Performing some work...\n");
    sleep(20);

    printf("Releasing the semaphore.\n");
    if (binary_semaphore_post(semid) < 0) 
    {
        perror("Semaphore post failed");
        exit(1);
    }

    // Deallocate the semaphore when done.
    if (binary_semaphore_deallocate(semid) < 0) 
    {
        perror("Semaphore deallocation failed");
        exit(1);
    }

    printf("Semaphore deallocated. Done.\n");

    return 0;
}