/**
 * Manipulating the cancellablity of the threads
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    int oldstate;
    int type;
    
    // Get the current cancelability type
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);

    printf("Thread is running with cancelability type %s\n", (oldstate == PTHREAD_CANCEL_DEFERRED) ? "PTHREAD_CANCEL_DEFERRED" : "PTHREAD_CANCEL_ASYNCHRONOUS");

    // Change the cancelability type to asynchronous
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &type);

    printf("Changed cancelability type to PTHREAD_CANCEL_ASYNCHRONOUS\n");

     // Set cancelability state to enabled
    int old_cancel_state;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel_state);   //PTHREAD_CANCEL_DISABLE to make code uncanceable -> critical sections of the code go into such uncancelable threads
    

    // This loop is not a cancellation point, but we can check for cancellation
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);

        // Check for pending cancellation
        pthread_testcancel();

        usleep(1000000);
    }

    return NULL;
}

int main() {
    pthread_t thread;
    
    // Create a thread with default attributes
    pthread_create(&thread, NULL, thread_function, NULL);

    // Let the thread run for a while
    sleep(2);

    // Send a cancellation request to the thread
    pthread_cancel(thread);
    
    // Wait for the thread to finish
    pthread_join(thread, NULL);
    
    printf("Thread finished.\n");
    
    return 0;
}
