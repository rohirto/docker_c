/**
 * To create other types of Mutexes
 * 3 Types of Mutexes:
 *      1. Locking Fast Mutex (Default) -> if a thread tries to lock the mutex again it will blocked until mutex is released by self (Which will never occur)
 *      2. Locking Recursive Mutex - can be safely locked many times by the same thread 
 *      3. Error Checking Mutex - the second call to the locked mutex will return failure code of EDEADLK
 * TO create 2 and 3 type of Mutex, we need to tweak with Mutex attribute object
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_mutexattr_t attr;

void* thread_function(void* arg) {
    //Try to lock the mutex
    //pthread_mutex_lock(&mutex);  //Uncommenting this will cause a deadlock
    
    int result = pthread_mutex_lock(&mutex);
    if (result != 0) {
        if (result == EDEADLK) {
            fprintf(stderr, "Thread %ld attempted to deadlock.\n", (long)arg);
            pthread_exit(NULL);
        } else if (result == EBUSY) {
            fprintf(stderr, "Thread %ld attempted to lock a busy mutex.\n", (long)arg);
        }
    } else {
        // Simulate some work
        printf("Thread %ld is working...\n", (long)arg);
        sleep(10);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Initialize the mutex with PTHREAD_MUTEX_ERRORCHECK_NP or PTHREAD_MUTEX_RECURSIVE_NP
    pthread_mutexattr_init(&attr);
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    // Create two threads
    if (pthread_create(&thread1, NULL, thread_function, (void*)1) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, thread_function, (void*)2) != 0) {
        perror("pthread_create");
        exit(1);
    }

    

    // Wait for the threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
