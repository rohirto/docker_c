#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


/* Function that prints 'x's to stderr. The parameter is unused. Does not return. */
void *print_xs(void *unused) {
    int i = 0;
    while (i++ < 1000) {
        fputc('x', stderr);
        sleep(1);
    }
    return NULL;
}

/* The main program. */
int main() {
    pthread_t thread_id;
    
    /* Create a new thread. The new thread will run the print_xs function. */
    pthread_create(&thread_id, NULL, &print_xs, NULL);
    int j = 0;
    /* Print 'o's continuously to stderr. */
    while (j++ < 1000) {
        fputc('o', stderr);
        sleep(1);
    }
    
    return 0;
}
