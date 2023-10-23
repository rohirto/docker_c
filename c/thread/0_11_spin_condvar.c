/**
 * Condition Variables - Flags to control the operation of threads
 * Spinning in a loop example below 
 * 
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int thread_flag;
pthread_mutex_t thread_flag_mutex;

void initialize_flag() 
{
    pthread_mutex_init(&thread_flag_mutex, NULL);  //Default mutex type
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
/* Calls do_work repeatedly while the thread flag is set; otherwise spins. */
void* thread_function(void* thread_arg) 
{
    while (1) 
    {
        int flag_is_set;

        /* Protect the flag with a mutex lock. */
        pthread_mutex_lock(&thread_flag_mutex);
        flag_is_set = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

        if (!flag_is_set)
            do_work(); // Replace this with the actual work function
        /* Else don't do anything. Just loop again. */
        else
            break;
    }
    return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE. */
void set_thread_flag(int flag_value) 
{
    /* Protect the flag with a mutex lock. */
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
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
    
    // Set the flag to stop the thread
    set_thread_flag(1);
    
    // Wait for the thread to finish
    pthread_join(thread_id, NULL);
    
    return 0;
}
