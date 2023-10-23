/**
 * In GNU/Linux Threads are implemented as processes (its different in other UNIX systems).
 * But 1 imp thing difference betn fork() and pthread_create is that former creates a copy of memory space (sepeate mem space for processes)
 * While latter shares the memory space with each other
 * 
 * run the ps command to see the pid 
 * 3 threads are running
 *      1. Main thread 
 *      2. Manager Thread - Created by Linux (system implementation)
 *      3. User Created thread - created using pthread_create
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg)
{
    fprintf(stderr, "child thread pid is %d\n", (int)getpid());
    /* Spin forever. */
    while (1);
    return NULL;
}

int main()
{
    pthread_t thread;
    fprintf(stderr, "main thread pid is %d\n", (int)getpid());
    pthread_create(&thread, NULL, &thread_function, NULL);
    /* Spin forever. */
    while (1);
    return 0;
}
