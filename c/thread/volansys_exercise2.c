/**
 * Develop a two-threaded application, wherein each thread writes numbers from (1 to 1000) in a same file. 
 * Thread one should prefix “#” and thread 2 should prefix “$” before writing numbers. Make sure at a time only one thread can write to a file. 
 * Also design this application in such a way that both threads run in parallel. 
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_OF_THREADS   2

const char* FILE_PATH="/tmp/tmpfile";
/* A mutex protecting shared resource. */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Parameters to char_print function. */
typedef struct char_print_parms
{
    /* The character to print. */
    char character;
    /* The number of times to print it. */
    int count;
}Print_params;

/* Prints a number of characters to stderr, as given by PARAMETERS,
which is a pointer to a struct char_print_parms. */
void* char_print(void* parameters) 
{
    pthread_mutex_lock(&mutex);
    /* Cast the cookie pointer to the right type. */
    Print_params* p = (Print_params*)parameters;
    FILE *fd = fopen(FILE_PATH,"a+");
    while(1)
    {
        if(p->count-- > 0)
        {
            char buff[5];
            sprintf(buff,"%c%d",p->character,(1000 - p->count));
            fputs(buff,fd);
            // fputc(p->character, fd);
            // putw(i,fd);
            // fwrite(&p->character,sizeof(char),1,fd);
            // fwrite(&i,sizeof(int),1,fd);
        }
        else
        {
            break;
        }
    }

    fclose(fd);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t thread_id_1;
    Print_params* p = (Print_params*)malloc(sizeof(Print_params));
    p->character = '#';
    p->count = 1000;
    pthread_create(&thread_id_1, NULL, &char_print, (void*)p);

    pthread_t thread_id_2;
    Print_params* q = (Print_params*)malloc(sizeof(Print_params));
    q->character = '$';
    q->count = 1000;
    pthread_create(&thread_id_2, NULL, &char_print, (void*)q);

   

    /* Wait for the prime number thread to complete and get the result. */
    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);
    
    free(p);
    free(q);
    
    return 0;
}

