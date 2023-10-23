/**
 * The function pthread_cleanup_push is used to ensure that resources are freed when a thread exits or is cancelled
 * Good practice to have it
 * push is balanced by pop -> to which is non zero value is passed then cleans up/ frees up the resources and unregisters the handle
 * 
*/
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


/* Allocate a temporary buffer. */
void* allocate_buffer(size_t size) 
{
    return malloc(size);
}

/* Deallocate a temporary buffer. */
void deallocate_buffer(void* buffer) 
{
    free(buffer);
    printf("Deallocated the buffer, freed the resources, no mem leak!\n");
}

void* do_some_work() 
{
    int oldstate;
    int type;
    /* Allocate a temporary buffer. */
    printf("Allocating the temp buffer\n");
    void* temp_buffer = allocate_buffer(1024);

    /* Register a cleanup handler for this buffer, to deallocate it in
       case the thread exits or is canceled. */
    pthread_cleanup_push(deallocate_buffer, temp_buffer);

    /* Do some work here that might call pthread_exit or might be
       canceled... */
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
    for (int i = 0; i < 10; i++)
     {
        printf("Working... %d\n", i);

        // Check for pending cancellation
        pthread_testcancel();

        usleep(1000000);
    }


    /* Unregister the cleanup handler. Because we pass a nonzero value,
       this actually performs the cleanup by calling deallocate_buffer. */
    pthread_cleanup_pop(1);
}
int main()
{
    pthread_t thread_id;

    /* Create a new thread. The new thread will run the print_xs function. */
    pthread_create(&thread_id, NULL, &do_some_work, NULL);
    // Let the thread run for a while
    sleep(2);

    // Send a cancellation request to the thread
    pthread_cancel(thread_id);
    
    // Wait for the thread to finish
    pthread_join(thread_id, NULL);
    
    printf("Thread finished.\n");
    

    return 0;
}