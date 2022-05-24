#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

#define SEM_NAME    "/chat"
#define BUFFER_SIZE 100

typedef struct message {
    char message[BUFFER_SIZE];
    int counter;
} message_t;

sem_t *sem;
message_t *msg;


_Noreturn void* listener() {
    int bufCounter = 0;
    while (1) {
        sem_wait(sem);
        if (msg->counter != bufCounter) {
            printf("Message: %s\n", msg->message);
            bufCounter = msg->counter;
        }
        sem_post(sem);
    }
}


int main(int argc, char *argv[]) {

    sem_unlink(SEM_NAME);
    if (argc < 2) {
        fputs("No file path", stderr);
        exit(EXIT_FAILURE);
    }

    const char *filepath = argv[1];
    char buffer[BUFFER_SIZE];
    int fd;

    pthread_t thread;

    sem = sem_open(SEM_NAME, O_RDWR | O_CREAT, 0777, 1);
    if (sem == SEM_FAILED) {
        fputs("Semaphore can not be opened", stderr);
        exit(errno);
    }

    fd = open(filepath, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "File '%s' can not be opened\n", filepath);
        exit(errno);
    }

    msg = mmap(0, sizeof(message_t), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (msg == MAP_FAILED) {
        printf("Mapping failed\n");
        exit(errno);
    }
    msg->counter = 0;
    close(fd);

    pthread_create(&thread, NULL, listener, NULL);

    do {
        fgets(buffer, BUFFER_SIZE, stdin);

        sem_wait(sem);
        strcpy(msg->message, buffer);
        msg->counter++;
        sem_post(sem);
    } while (strcmp(buffer, "q\n") != 0);

    sem_unlink(SEM_NAME);

    return EXIT_SUCCESS;
}
