/**
 * Setting the attributes of the thread, 
 * 2 types of threads -> joinable thread (which can be converted to detached thread) -> needs to have pthread_join
 *                       detached thread -> no need for pthread_join
 * Once detached thread -> cannot convert it to joinable thread
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Function that will be executed by the new thread
void* thread_function(void* thread_arg) 
{
    /* Do work here... */
    printf("Thread running\n");
    sleep(2);

}

int main() {
    // Initialize thread attributes
    pthread_attr_t attr;
    pthread_t thread;

    pthread_attr_init(&attr);

    // Set the thread as detached (it will automatically clean up when it finishes)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Create a new thread with the specified attributes
    pthread_create(&thread, &attr, &thread_function, NULL);

    // Destroy the thread attributes as they are no longer needed
    pthread_attr_destroy(&attr);

    /* Do work in the main thread... */
    printf("This is the Main Thread\n");
    sleep(5);
    printf("Exiting code \n");

    /* No need to join the second thread, as it's detached. */
    return 0;
}
