//Write a program to calculate the sum of an array using multiple threads.
#include <stdio.h>
#include <pthread.h>

#define ARRAY_SIZE 100000
#define NUM_THREADS 4

int array[ARRAY_SIZE];
int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *partialSum(void *arg) {
    int threadId = *(int *)arg;
    int startIndex = threadId * (ARRAY_SIZE / NUM_THREADS);
    int endIndex = startIndex + (ARRAY_SIZE / NUM_THREADS);

    int localSum = 0;
    for (int i = startIndex; i < endIndex; i++) {
        localSum += array[i];
    }

    pthread_mutex_lock(&mutex);
    sum += localSum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, partialSum, &threadIds[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Sum: %d\n", sum);

    return 0;
}