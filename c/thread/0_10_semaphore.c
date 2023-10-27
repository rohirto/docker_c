/**
 * In example 0_7_mutexs.c, If the job queue becomes empty, the thread exits and the application ends
 * But what if we anticipate that further jobs will be added in this queue and thread must go into a blocking state and execute tasks again 
 * when new jobs arrive
 * 
 * To acheive above task semaphores are used
 * every Semaphr has a counter value
 * Supports 2 operation 
 *      1. Wait op - decrement the counter by 1 till it becomes 0, block threads from here, until the counter again becomes positive
 *      2. Post op - increments the value of counter by 1
 * 2 implementations
 *      1. Communication among threads  - explained here   -- POSIX semaphore
 *      2. Communication among Processes -will be explained in IPC  -- System V semaphore
 * 
 * This code also consists example of Conditional Variable (A flag to control the behaviour of thread)
 * A smart technique to exit a thread waiting on a sempahore is used: 
 *      Increment the semaphore count and use conditional variable to exit the threads
*/

#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>  //POSIX Semaphore, different implementation than <sys/sem.h> which is System V semaphore
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int thread_flag;
pthread_mutex_t thread_flag_mutex;

/* Function to initialize conditional Variable */
void initialize_flag() 
{
    pthread_mutex_init(&thread_flag_mutex, NULL);  //Default mutex type
    thread_flag = 0;
}


struct job 
{
    /* Link field for linked list. */
    struct job* next;
    /* Other fields describing work to be done... */
    const char* msg;
    int thread_no;
};

/* A linked list of pending jobs. */
struct job* job_queue;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* A semaphore counting the number of jobs in the queue. */
sem_t job_queue_count;


/* Sets the value of the thread flag to FLAG_VALUE. */
void set_thread_flag(int flag_value) 
{
    /* Protect the flag with a mutex lock. */
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    if (flag_value) 
    {
        sem_post(&job_queue_count); //To remove thread from waiting stage
    }
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* Simulated work to be done for a job. */
void process_job(struct job* job) 
{
    printf("Processing job...\n");
    // Simulate some work here
    printf("Thread no: %d, msg: %s\n",job->thread_no, job->msg);
    sleep(1); // Sleep for 1 second
    printf("Job processed.\n");
}

/* Perform one-time initialization of the job queue. */
void initialize_job_queue() 
{
    /* The queue is initially empty. */
    job_queue = NULL;
    /* Initialize the semaphore which counts jobs in the queue. Its
       initial value should be zero. */
    sem_init(&job_queue_count, 0, 0);
}

/* Process queued jobs until the queue is empty. */
void* thread_function(void* arg) 
{
    
    while (1) 
    {
       int flag_is_set;

        /* Protect the flag with a mutex lock. */
        pthread_mutex_lock(&thread_flag_mutex);
        flag_is_set = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

       
        struct job* next_job;

        /* Wait on the job queue semaphore. If its value is positive,
        indicating that the queue is not empty, decrement the count by
        1. If the queue is empty, block until a new job is enqueued. */
        sem_wait(&job_queue_count);  //For Blocking semaphore wait
        //sem_trywait(&job_queue_count); //For Non Blocking semaphore wait

        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);

        /* Because of the semaphore, we know the queue is not empty. Get
        the next available job. */
        next_job = job_queue;

        /* Remove this job from the list. */
        job_queue = job_queue->next;

        /* Unlock the mutex on the job queue because we're done with the
        queue for now. */
        pthread_mutex_unlock(&job_queue_mutex);


        /* Carry out the work. */
        process_job(next_job);

        

        /* Clean up. */
        free(next_job);
        if (flag_is_set)
        {
            break;
        }

    }

    return NULL;
}

/* Add a new job to the front of the job queue. */
void enqueue_job(int i, const char* msg) 
{
    struct job* new_job;
    /* Allocate a new job object. */
    new_job = (struct job*)malloc(sizeof(struct job));
    /* Set the other fields of the job struct here... */
    new_job->thread_no = i;
    new_job->msg = msg;

    /* Lock the mutex on the job queue before accessing it. */
    pthread_mutex_lock(&job_queue_mutex);

    /* Place the new job at the head of the queue. */
    new_job->next = job_queue;
    job_queue = new_job;

    /* Post to the semaphore to indicate that another job is available. If
       threads are blocked, waiting on the semaphore, one will become
       unblocked so it can process the job. */
    sem_post(&job_queue_count);

    /* Unlock the job queue mutex. */
    pthread_mutex_unlock(&job_queue_mutex);
}

// To take user input in realtime, 
void* user_input_thread(void* arg) 
{
    char input_buffer[256];

    while (1) 
    {
        printf("Enter a message (or 'q' to quit): ");
        fflush(stdout);
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) 
        {
            perror("Error reading input");
            exit(1);
        }
        input_buffer[strlen(input_buffer) - 1] = '\0'; // Remove the newline character

        if (strcmp(input_buffer, "q") == 0) 
        {
            printf("Exiting user input thread.\n");
            break;
        }

        enqueue_job(0, input_buffer);  //thread 0 shows user input
    }
    
    // Set the flag to stop the thread
    set_thread_flag(1);
    return NULL;
}

int main() 
{
    initialize_job_queue();
    
    // Simulate adding jobs to the queue.
    for (int i = 1; i <= 5; i++) 
    {
        enqueue_job(i, "Hello from thread");
        printf("Job %d enqueued.\n", i);

    }

    // Create a user input thread to handle real-time input.
    pthread_t user_input_thread_id;
    pthread_create(&user_input_thread_id, NULL, user_input_thread, NULL);

    // Create a threads to process jobs.
    pthread_t worker_thread1, worker_thread2;

    pthread_create(&worker_thread1, NULL, thread_function, NULL);
    pthread_create(&worker_thread2, NULL, thread_function, NULL);
    // Sleep briefly to simulate a job queue.
    sleep(2);

    // Wait for the worker thread to finish.
    pthread_join(worker_thread1, NULL);
    pthread_join(worker_thread2, NULL);

    // Wait for the user input thread to finish.
    pthread_join(user_input_thread_id, NULL);

    return 0;
}

