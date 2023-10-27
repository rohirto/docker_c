/**
 *  Develop a parent-child process application, wherein child process at random duration calculates square of numbers (via above method) 
 * from 1 to 1000 and parent process prints result as soon as soon as child process produce it. Use Shared memory for IPC (10 points)
 * 
 * Will be using binary semaphore to sync access to shared mem,
 * System V Semaphore Implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdbool.h>

//Whether to use Delay
#define USE_DELAY   0

//the key for shared memory and semaphores
#define SHARED_MEMORY_KEY 1234
#define SEMAPHORE_KEY 5678

//Earlier Defines
#define CHILD_PROCESS   (pid_t)0
#define COUNT           1000


// Semaphore operations
struct sembuf sem_lock = {1, -1, SEM_UNDO}; // Lock the shared resource
struct sembuf sem_unlock = {1, 1, SEM_UNDO}; // Unlock the shared resource


struct shared_mem
{
    int sq[COUNT];
};



int square_calculater(int number) 
{
    int square = 0;
    // Calculate the square using AP
    for (int j = number; j > 0; j--)
    {
        square += number;
    }

    return square;
}

int main()
{
    pid_t pid;

    // Create or get the shared memory segment
    int shmid = shmget(SHARED_MEMORY_KEY, sizeof(struct shared_mem), IPC_CREAT | 0666);
    if (shmid == -1) 
    {
        perror("Shared memory creation failed");
        exit(1);
    }

    // Attach the shared memory segment
    struct shared_mem* squares = (struct shared_mem*)shmat(shmid, NULL, 0);
    if (squares == (struct shared_mem*) -1) 
    {
        perror("Shared memory attachment failed");
        exit(1);
    }

    // Create or get the semaphore
    int semid = semget(SEMAPHORE_KEY, 2, IPC_CREAT | 0666);  //key, no of semaphores, semflag
    if (semid == -1) 
    {
        perror("Semaphore creation failed");
        exit(1);
    }

    //POSIX Sem implementation
    // Initialize the first semaphore for synchronization
    union semun
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_arg;

    sem_arg.val = 0;  // Initialize to 0
    semctl(semid, 0, SETVAL, sem_arg);

    // Initialize the second semaphore for mutual exclusion
    sem_arg.val = 1;  // Initialize to 1
    semctl(semid, 1, SETVAL, sem_arg);

    //Forking process begins from here
    /* Fork a child process. */
    pid = fork();

    if(pid == CHILD_PROCESS)
    {
        //Child Process
        
        // Produce an item and add it to the shared mem
        for (int i = 1; i <= COUNT; i++)
        {
            #if USE_DELAY
            // Simulate a random delay
            int delay = rand() % 1000000; // Up to 1 second delay
            usleep(delay);
            #endif
            
            // // Wait for the semaphore
            // semop(semid, &sem_wait, 1);
            // Wait for the synchronization semaphore (semaphore 0)
            semop(semid, &sem_lock, 1);

            squares->sq[i-1] = square_calculater(i);

            // // Signal the semaphore
            // semop(semid, &sem_signal, 1);
            semop(semid, &sem_unlock, 1);

        }
        
    }
    else if (pid > CHILD_PROCESS)
    {
        
        
        //Parent Process
       for (int i = 0; i < COUNT; i++)
        {
            // // Wait for the semaphore
            // semop(semid, &sem_wait, 1);
            

            while(squares->sq[i] == 0)
            {
                #if USE_DELAY
                usleep(1000); // Sleep for 1 millisecond
                #endif
            }
            semop(semid, &sem_lock, 1);
            printf("%d\n", squares->sq[i]);

            // // Signal the semaphore
            // semop(semid, &sem_signal, 1);
            semop(semid, &sem_unlock, 1);
        }

        // Parent process waits for children
        wait(NULL);

        
    }
    else
    {
        perror("fork");
        exit(1);
    }

    // Detach and remove shared memory and semaphore
    shmdt(squares);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);



    return 0;
}

