#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include "circle.h"

struct Circle circle;

pthread_mutex_t mutex;

sem_t *sem_consumer;
sem_t *sem_producer;


_Noreturn void* producerFunction() {
    while (1) {
        sem_wait(sem_producer);
        pthread_mutex_lock(&mutex);

        int size = rand() % 253;
        for (int i = 0; i < size; i++) {
            int letter = rand() % 26;
            circle.message[circle.head].string[i] = (char)(letter + 96);
        }
        circle.message[circle.head].string[size] = '\0';
        circle.added++;
        printf("Producer generated message\n");

        if (circle.head == CAPACITY - 1) {
            circle.head = 0;
        } else {
            circle.head++;
        }

        pthread_mutex_unlock(&mutex);

        sem_post(sem_consumer);
        usleep(1000000);
    }
}

_Noreturn void* consumerFunction() {
    while (1) {
        sem_wait(sem_consumer);
        pthread_mutex_lock(&mutex);

        printf("Consumer read message: %s\n", circle.message[circle.tail].string);

        for (int i = 0; i < 254; i++)
            circle.message[circle.tail].string[i] = '\0';

        circle.deleted++;

        if (circle.tail == CAPACITY - 1) {
            circle.tail = 0;
        } else {
            circle.tail++;
        }

        pthread_mutex_unlock(&mutex);

        sem_post(sem_producer);
        usleep(1000000);
    }
}


int main() {

    sem_consumer = sem_open("sem_consumer", O_CREAT | O_RDWR, 0644, 0);
    sem_producer = sem_open("sem_producer", O_CREAT | O_RDWR, 0644, CAPACITY);

    pthread_mutex_init(&mutex, NULL);

    int consumerCount = 0, producerCount = 0;

    circle.added = 0;
    circle.deleted = 0;
    circle.head = 0;
    circle.tail = 0;

    pthread_t producers[CAPACITY];
    pthread_t consumers[CAPACITY];

    int choice;
    while (1) {
        choice = getchar();

        switch (choice) {
            case 's':
                printf("Stats:\n"
                       "Added: %d\n"
                       "Deleted: %d\n",
                       circle.added,
                       circle.deleted);
                break;
            case 'p':
                if (pthread_create(&producers[producerCount], NULL, producerFunction, NULL) != 0) {
                    printf("Pthread_create: Error - %d\n", errno);
                    exit(errno);
                }
                producerCount++;
                break;
            case 'c':
                if (pthread_create(&consumers[consumerCount], NULL, consumerFunction, NULL) != 0) {
                    printf("Pthread_create: Error - %d\n", errno);
                    exit(errno);
                }
                consumerCount++;
                break;
            case 'P':
                if (producerCount > 0) {
                    pthread_mutex_lock(&mutex);
                    pthread_cancel(producers[producerCount - 1]);
                    producerCount--;
                    pthread_mutex_unlock(&mutex);
                    printf("Producer deleted\n");
                } else {
                    printf("No producers\n");
                }
                break;
            case 'C':
                if (consumerCount > 0) {
                    pthread_mutex_lock(&mutex);
                    pthread_cancel(consumers[consumerCount - 1]);
                    consumerCount--;
                    pthread_mutex_unlock(&mutex);
                    printf("Consumer deleted\n");
                } else {
                    printf("No consumers\n");
                }
                break;
            case 'q':
                pthread_mutex_destroy(&mutex);
                sem_unlink("sem_consumer");
                sem_unlink("sem_producer");
                return 0;
            default:
                break;
        }
    }
}