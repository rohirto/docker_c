// Write a C program to create two threads that independently print "Hello" and "World" alternately.
// No sync Mechanism, thus behaviour of printing Hello world cant be determined

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  //Sleep

void *printHello(void *arg) {
    while (1) {
        printf("Hello ");
        fflush(stdout);
        sleep(1); // Sleep for 1 second
    }
    return NULL;
}

void *printWorld(void *arg) {
    while (1) {
        printf("World\n");
        fflush(stdout);
        sleep(1); // Sleep for 1 second
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, printHello, NULL);
    pthread_create(&thread2, NULL, printWorld, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}