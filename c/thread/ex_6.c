/*
Using _Atomic and thread_local with the pthread library can help ensure thread safety and provide thread-local storage, 
respectively. Here's an example that demonstrates the use of _Atomic for atomic operations and thread_local for thread-local 
storage with pthreads:

In this example:

We define a shared _Atomic integer variable atomic_counter that will be incremented using atomic operations to ensure thread safety.

We also define a thread-local integer variable thread_local_counter using the thread_local keyword. Each thread will have its own copy of this variable.

The thread_function increments the shared atomic counter by 10,000 using atomic operations and increments the thread-local counter by 10,000.

In the main function, we create three threads using pthread_create, each with a unique thread ID.

After all threads finish, we print the final value of the shared atomic counter from the main thread.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 3

// Shared atomic counter variable
_Atomic int atomic_counter = 0;

// Thread-local counter variables
_Thread_local int thread_local_counter = 0;

// Thread function
void* thread_function(void* arg) {
    int thread_id = *((int*)arg); // Cast and dereference the argument

    // Increment the shared atomic counter using atomic operations
    for (int i = 0; i < 10000; i++) {
        atomic_fetch_add(&atomic_counter, 1);
    }

    // Increment the thread-local counter
    thread_local_counter += 10000;

    printf("Thread %d: Shared Atomic Counter = %d, Thread-Local Counter = %d\n", thread_id, atomic_counter, thread_local_counter);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    printf("Main Thread: Final Shared Atomic Counter = %d\n", atomic_counter);

    return 0;
}