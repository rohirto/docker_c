/**
 * If a thread tries to access a locked mutex then that thread will be kept in blocked state, 
 * If we want to avoid this because the thread can perform some other task then non blocking mutex methods are implemented
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void* myThread(void* arg) 
{
    // Attempt to lock the mutex using pthread_mutex_trylock
    int result = pthread_mutex_trylock(&myMutex);
    
    if (result == 0) 
    {
        // Mutex successfully locked
        printf("Thread %d acquired the mutex.\n", (int)arg);
        // Simulate some work
        sleep(1);
        // Unlock the mutex
        pthread_mutex_unlock(&myMutex);
        printf("Thread %d released the mutex.\n", (int)arg);
    } else if (result == EBUSY) 
    {
        // Mutex is already locked by another thread
        printf("Thread %d couldn't acquire the mutex. It's already locked.\n", (int)arg);
    } else 
    {
        // An error occurred
        printf("Thread %d encountered an error while trying to lock the mutex.\n", (int)arg);
    }

    return NULL;
}

int main() 
{
    pthread_t threads[5];
    
    for (int i = 0; i < 5; i++) 
    {
        pthread_create(&threads[i], NULL, myThread, (void*)i);
    }

    for (int i = 0; i < 5; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&myMutex);

    return 0;
}
