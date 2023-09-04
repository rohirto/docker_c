#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
#define ITERATIONS 10000

// Shared counter variable
int counter = 0;

// Mutex to protect the shared counter
pthread_mutex_t mutex;

// Thread function to increment the counter
void* increment(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        // Lock the mutex before accessing the shared counter
        pthread_mutex_lock(&mutex);

        counter++; // Increment the counter

        // Unlock the mutex after updating the counter
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Thread IDs
    pthread_t threads[NUM_THREADS];

    // Create two threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for both threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    // Print the final value of the counter
    printf("Final counter value: %d\n", counter);

    return 0;
}