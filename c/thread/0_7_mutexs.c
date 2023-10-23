/**
 * Using Mutual exculsion locks
 * More threads, quick job doing, but then synchronization needed to do multithreading
 * Can lead to Dead locks -> where 1 thread can block other thread - 1 or more threads waiting for spmething that will never occur
 * 
 * 
*/
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


struct job 
{
    /* Link field for the linked list. */
    struct job* next;
    /* Other fields describing work to be done... */
    const char* msg;
    int thread_no;
};

/* A linked list of pending jobs. */
struct job* job_queue = NULL;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Simulated work to be done for a job. */
void process_job(struct job* job) 
{
    printf("Processing job...\n");
    // Simulate some work here
    printf("Thread no: %d, msg: %s",job->thread_no, job->msg);
    sleep(1); // Sleep for 1 second
    printf("Job processed.\n");
}

/* Add a job to the queue. */
void enqueue_job(struct job* new_job) 
{
    pthread_mutex_lock(&job_queue_mutex);

    new_job->next = job_queue;
    job_queue = new_job;

    pthread_mutex_unlock(&job_queue_mutex);
}

/* Process queued jobs until the queue is empty. */
void* thread_function(void* arg) 
{
    while (1) 
    {
        struct job* next_job = NULL;

        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);

        /* Now it's safe to check if the queue is empty. */
        if (job_queue == NULL)
            next_job = NULL;
        else 
        {
            /* Get the next available job. */
            next_job = job_queue;

            /* Remove this job from the list. */
            job_queue = job_queue->next;
        }

        /* Unlock the mutex on the job queue because we're done with the queue for now. */
        pthread_mutex_unlock(&job_queue_mutex);

        /* Was the queue empty? If so, end the thread. */
        if (next_job == NULL)
            break;

        /* Carry out the work. */
        process_job(next_job);

        /* Clean up. */
        free(next_job);
    }

    return NULL;
}

int main() 
{
    
    // Simulate adding jobs to the queue.
    for (int i = 1; i <= 5; i++) 
    {
        struct job* new_job = (struct job*)malloc(sizeof(struct job));
        new_job->next = NULL;
        // You can add more fields and data to the new_job structure.
        new_job->thread_no = i;
        new_job->msg = "Hello from thread\n";
        // Enqueue the job for processing.
        enqueue_job(new_job);

        printf("Job %d enqueued.\n", i);

        
    }

    // Create a threads to process jobs.
    pthread_t worker_thread1, worker_thread2;

    pthread_create(&worker_thread1, NULL, thread_function, NULL);
    pthread_create(&worker_thread2, NULL, thread_function, NULL);
    // Sleep briefly to simulate a job queue.
    sleep(2);

    // Wait for the worker thread to finish.
    pthread_join(worker_thread1, NULL);
    pthread_join(worker_thread2, NULL);

    return 0;
}
