//Pthread pthread_join example

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  //Sleep
#include <stdlib.h>

void* thread_function(void* arg) {
    int thread_id = *((int*)arg); // Cast and dereference the argument

    printf("Thread %d is executing.\n", thread_id);

    // Simulate some work
    for (int i = 0; i < 3; i++) {
        printf("Thread %d is working...\n", thread_id);
        sleep(1); // Sleep for 1 second
    }

    printf("Thread %d is done.\n", thread_id);

    // Return an exit status (can be NULL)
    int* exit_status = malloc(sizeof(int));
    *exit_status = thread_id;
    pthread_exit(exit_status);
}

int main() {
    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    for (int i = 0; i < 2; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Wait for both threads to finish
    for (int i = 0; i < 2; i++) {
        int* exit_status;
        if (pthread_join(threads[i], (void**)&exit_status) != 0) {
            perror("pthread_join");
            return 1;
        }
        printf("Thread %d exited with status %d.\n", i + 1, *exit_status);
        free(exit_status);
    }

    printf("All threads have finished.\n");

    return 0;
}
