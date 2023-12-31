/**
 * Next Level use of Conditional Variable
 * Using Conditional Variable along with Mutexes
 * Picking up the example from 0_11_spincondvar.c, another way to implemment it
 * The previous example - possibility of race condition, which is eleminated here
 * More control over the functioning of threads using function set_thread_flag
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

void initialize_flag()
{
    /* Initialize the mutex and condition variable. */
    pthread_mutex_init(&thread_flag_mutex, NULL);
    pthread_cond_init(&thread_flag_cv, NULL);
    /* Initialize the flag value. */
    thread_flag = 0;
}
void do_work() 
{
    // Replace this with the actual work that the thread should perform.
    // You can implement the work logic here.
    // For example:
    printf("Thread is doing some work...\n");
    sleep(1); // Simulate some work for 1 second
}
/* Calls do_work repeatedly while the thread flag is set; blocks if
   the flag is clear. */
void* thread_function(void* thread_arg)
{
    /* Loop infinitely. */
    while (1) 
    {
        /* Lock the mutex before accessing the flag value. */
        pthread_mutex_lock(&thread_flag_mutex);
        while (!thread_flag) 
        {
            /* The flag is clear. Wait for a signal on the condition
               variable, indicating that the flag value has changed. When the
               signal arrives and this thread unblocks, loop and check the
               flag again. */
            pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);
        }
        /* When we’ve gotten here, we know the flag must be set. Unlock
           the mutex. */
        pthread_mutex_unlock(&thread_flag_mutex);
        /* Do some work. */
        do_work();
    }
    return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE. */
void set_thread_flag(int flag_value)
{
    /* Lock the mutex before accessing the flag value. */
    pthread_mutex_lock(&thread_flag_mutex);
    /* Set the flag value, and then signal in case thread_function is
       blocked, waiting for the flag to become set. However,
       thread_function can’t actually check the flag until the mutex is
       unlocked. */
    thread_flag = flag_value;
    pthread_cond_signal(&thread_flag_cv);
    /* Unlock the mutex. */
    pthread_mutex_unlock(&thread_flag_mutex);
}
int main() 
{
    initialize_flag();
    
    // Start the thread
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_function, NULL);
    
    // Let the thread work for a while
    sleep(5);
    
    // Set the flag to start the thread
    set_thread_flag(1);

    sleep(5);

    //To stop the thread
    set_thread_flag(0);
    
    // Wait for the thread to finish
    pthread_join(thread_id, NULL);
    
    return 0;
}