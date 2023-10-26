/**
 * This are some synchronozation mechanism which are not given in the book
 * 
 * Spinlocks: A spinlock is a simple synchronization mechanism that causes a thread to repeatedly check for a lock (spin) until it becomes available.
 * Please note that spinlocks are not always the best choice for synchronization, especially in situations where threads might have to wait for a long time.
 *  In such cases, other synchronization mechanisms like mutexes or condition variables might be more appropriate.
 * 
 * Spinning:
 * If the spinlock is already in a locked state (i.e., another thread holds it), the thread doesn't block or yield. 
 * Instead, it keeps spinning or busy-waiting in a loop, continuously checking the spinlock's state.
 * The spinning thread repeatedly checks the spinlock without relinquishing the CPU. 
 * This tight loop is where the term "spinlock" comes from.
 * 
 * Whether spinlocks are appropriate or not depends on the system's characteristics and usage context
 * Not suitable for Single Processor system 
 *  1. Wasted CPU time 
 *  2. Limited Concurrency
 *  3. Inefficent Waiting
 * 
 * Suitable for Multiprocessor system 
 *  1. Effective Use of Multiple Cores
 *  2. Lower Overhead - as compared to some other sync mechanism like mutextes and semaphores 
 *  3. Predictable Behaviour -  
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a global spinlock
pthread_spinlock_t lock;

// Function executed by multiple threads
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    
    // Spin until the lock becomes available
    while (pthread_spin_trylock(&lock) != 0) {
        // Spin (busy-wait)
    }

    printf("Thread %d has acquired the lock.\n", thread_id);
    
    // Simulate some work
    for (int i = 0; i < 1000000; i++);

    printf("Thread %d is releasing the lock.\n", thread_id);
    
    // Release the lock
    pthread_spin_unlock(&lock);
    
    return NULL;
}

int main() {
    const int num_threads = 4;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    // Initialize the spinlock
    pthread_spin_init(&lock, PTHREAD_PROCESS_SHARED);

    // Create and run multiple threads
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the spinlock
    pthread_spin_destroy(&lock);

    return 0;
}
