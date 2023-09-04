//Example of Semaphore

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 3
#define NUM_ITEMS 10

// Shared buffer and its index
int buffer[BUFFER_SIZE];
int buffer_index = 0;

// Semaphores
sem_t empty, full;

// Mutex to protect the buffer
pthread_mutex_t mutex;

// Producer function
void* producer(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100; // Simulate producing an item
        
        sem_wait(&empty); // Wait if the buffer is full
        pthread_mutex_lock(&mutex); // Lock the buffer
        
        // Add the item to the buffer
        buffer[buffer_index] = item;
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;
        printf("Produced: %d\n", item);
        
        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full); // Signal that the buffer is no longer empty
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&full); // Wait if the buffer is empty
        pthread_mutex_lock(&mutex); // Lock the buffer
        
        // Remove an item from the buffer
        int item = buffer[buffer_index];
        buffer_index = (buffer_index - 1 + BUFFER_SIZE) % BUFFER_SIZE;
        printf("Consumed: %d\n", item);
        
        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty); // Signal that the buffer is no longer full
    }
    return NULL;
}

int main() {
    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE); // Initially, the buffer is empty
    sem_init(&full, 0, 0); // Initially, there are no items in the buffer
    pthread_mutex_init(&mutex, NULL);

    // Thread IDs
    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_threads[NUM_CONSUMERS];

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        if (pthread_create(&producer_threads[i], NULL, producer, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        if (pthread_create(&consumer_threads[i], NULL, consumer, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for producer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        if (pthread_join(producer_threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for consumer threads to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        if (pthread_join(consumer_threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Clean up resources
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}