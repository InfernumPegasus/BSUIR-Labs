#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "circle.h"

/*
 * В 8 проверка очереди через семафор
 */

void readMessage(struct Circle **circle) {
    unsigned long long checksum = 0;

    for(int i = 0; (*circle)->message[i] != '\0'; i++) {
        int num = (*circle)->message[i];
        checksum = checksum + num * i;
    }

    if(checksum != (*circle)->checksum) {
        printf("Checksum is not correct!\n");
        return;
    }

    printf("Consumer read message: %s\n", (*circle)->message);
    for(int i = 0; i < 254; i++)
        (*circle)->message[i] = '\0';
    (*circle)->checksum = 0;
    (*circle)->deleted++;
}

int main(int argc, char *argv[]) {
    sem_t *semMutex = sem_open("semmutex",O_CREAT, 0644, 1);

    if(semMutex == SEM_FAILED) {
        perror("sem open failed");
        exit(errno);
    }

    struct Circle *circle = shmat(atoi(argv[0]), NULL, 0);
    int shmid;

    while(1) {
        sem_wait(semMutex);
        if(circle->message[0] != '\0') {
            readMessage(&circle);
            sem_post(semMutex);
            usleep(3000000);
        } else {
            sem_post(semMutex);
        }

        shmid = circle->right;
        shmdt(circle);
        shmat(shmid, NULL, 0);
        usleep(1000000);
    }

    shmdt(circle);

    sem_close(semMutex);
    return 0;
}
