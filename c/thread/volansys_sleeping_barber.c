/**
 * The Sleeping-Barber Problem. A barbershop consists of a waiting room with n chairs, and the barber room containing the barber chair. 
 * If there are no customers to be served, the barber goes to sleep. If a customer enters the barbershop and all chairs are occupied, 
 * then the customer leaves the shop. If the barber is busy, but chairs are available, then the customer sits in one of the free chairs. 
 * If the barber is asleep, the customer wakes up the barber. Write a pseudo program to co-ordinate the barber and the customers.
 * 
 * Problem can be solved using Semaphores
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CHAIRS 5

sem_t customers;
sem_t barber;
sem_t mutex;
int waiting = 0;

void *barber_thread(void *arg) 
{
    while (1) 
    {
        sem_wait(&customers); // Sleep if no customers
        sem_wait(&mutex);
        waiting--;
        sem_post(&barber); // Signal the barber is ready to cut hair
        sem_post(&mutex);

        printf("Barber is cutting hair.\n");
        sleep(2); // Simulate haircut
    }
}

void *customer_thread(void *arg) 
{
    int id = *((int *)arg);

    sem_wait(&mutex);
    if (waiting < MAX_CHAIRS) 
    {
        waiting++;
        printf("Customer %d takes a chair in the waiting room.\n", id);
        sem_post(&customers);
        sem_post(&mutex);

        sem_wait(&barber); // Wait for the barber
        printf("Customer %d is getting a haircut.\n", id);
        sleep(1); // Simulate haircut
        printf("Customer %d is done with the haircut.\n", id);
    } 
    else
     {
        printf("Customer %d couldn't find a chair and leaves.\n", id);
        sem_post(&mutex);
    }
}

int main() 
{
    pthread_t barber_tid;
    pthread_t customer_tid[10]; // 10 customers

    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&barber_tid, NULL, barber_thread, NULL);

    int customer_ids[10];
    for (int i = 0; i < 10; i++) 
    {
        customer_ids[i] = i;
        pthread_create(&customer_tid[i], NULL, customer_thread, &customer_ids[i]);
    }

    for (int i = 0; i < 10; i++) 
    {
        pthread_join(customer_tid[i], NULL);
    }

    return 0;
}
