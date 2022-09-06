#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "circle.h"

void generateMessage(struct Circle **circle) {
    int size = rand() % 253;

    for(int i = 0; i < size; i++) {
        int letter = rand() % 26;
        (*circle)->message[i] = letter + 96;
    }
    (*circle)->message[size] = '\0';

    for(int i = 0; i < size;i++) {
        int num = (*circle)->message[i];
        (*circle)->checksum = (*circle)->checksum + num * i;
    }

    (*circle)->added++;
    printf("Producer generated message\n");
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
        if(circle->message[0] == '\0') {
            generateMessage(&circle);
            sem_post(semMutex);
            usleep(3000000);
        } else {
            printf("Circle is full!\n");
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
