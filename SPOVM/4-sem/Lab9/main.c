#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>

struct message {
    char name[20];
    char message[100];
};

static int fd = 0;
struct message msg;


void *write_m(void * arg);
bool unique(struct message * msg1, struct message * msg2);

static pthread_mutex_t mutex_write;

int main() {

    pthread_t thread;
    struct message empty_msg = {""};

    fd = open("buffer", O_RDWR);
    if (fd < 0) {
        perror("File opened error..");
        exit(EXIT_FAILURE);
    }

//    printf("Enter name: ");
//    fgets(msg.name, 20, stdin);

    ftruncate (fd, sizeof(struct message)); //обрезка файла до нужного размера

    void * shared_memory = NULL;
    shared_memory = mmap(0, sizeof(struct message), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pthread_create(&thread, NULL, write_m, shared_memory);

    struct message temp_msg;

    while (true) {
        memcpy(&temp_msg, shared_memory, sizeof(struct message));
        if (unique((struct message *) &msg, (struct message *) &temp_msg) == true)
            continue;

        while (!strcmp(temp_msg.message, "")) {
            memcpy(&temp_msg, shared_memory, sizeof(struct message));
        }

        printf("\nMessage: %s\n", temp_msg.message);
        memcpy(shared_memory, &empty_msg, sizeof(struct message));
    }
}

void *write_m(void * arg) {
    while(true) {
        pthread_mutex_lock(&mutex_write);

        fflush(stdin);
        fgets(msg.name, 20, stdin);

        memcpy(arg, &msg, sizeof(struct message));
        pthread_mutex_unlock(&mutex_write);
    }
}


bool unique(struct message * msg1, struct message * msg2) {
    if (!strcmp(msg2 -> name, ""))
        return true;

    if (strcmp(msg1 -> name, msg2 -> name))
        return false;
    return true;
}



//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
//#include <unistd.h>
//#include <string.h>
//#include <fcntl.h>
//#include <sys/mman.h>
//#include <sys/shm.h>
//#include <semaphore.h>
//#include <pthread.h>
//
//#define SEM1_NAME    "/sem1"
//#define SEM2_NAME    "/sem2"
//#define BUFFER_SIZE 100
//
//typedef struct message {
//    char message[BUFFER_SIZE];
//    int counter;
//} message_t;
//
//sem_t *sem1;
//sem_t *sem2;
//message_t *msg;
//
//
//_Noreturn void* read_handler() {
//    int bufCounter = 0;
//    while (1) {
//        sem_post(sem1);
//        sem_wait(sem2);
//        if (msg->counter != bufCounter) {
//            printf("Message: %s\n", msg->message);
//            bufCounter = msg->counter;
//        }
//        sem_post(sem2);
//    }
//}
//
//
//int main() {
//
//    sem_unlink(SEM1_NAME);
//    sem_unlink(SEM2_NAME);
//
//    const char *filepath = "./memory";
//    char buffer[BUFFER_SIZE];
//    int fd;
////
////    int shm_id = 0;
////    int *processes = shmat(shm_id, NULL, 0);
////    if (processes == NULL) {
////        puts("NULL!");
////        exit(1);
////    }
////    *processes = *processes + 1;
////    printf("Processes: %d\n", *processes);
//
//    pthread_t thread;
//
//    sem1 = sem_open(SEM1_NAME, O_RDWR | O_CREAT, 0777, 1);
//    if (sem1 == SEM_FAILED) {
//        fputs("Semaphore1 can not be opened", stderr);
//        exit(errno);
//    }
//
//    sem2 = sem_open(SEM2_NAME, O_RDWR | O_CREAT, 0777, 1);
//    if (sem2 == SEM_FAILED) {
//        fputs("Semaphore2 can not be opened", stderr);
//        exit(errno);
//    }
//
//    fd = open(filepath, O_RDWR);
//    if (fd < 0) {
//        fprintf(stderr, "File '%s' can not be opened\n", filepath);
//        exit(errno);
//    }
//
//    msg = mmap(NULL, sizeof(message_t), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
//    if (msg == MAP_FAILED) {
//        printf("Mapping failed\n");
//        exit(errno);
//    }
//    msg->counter = 0;
//    close(fd);
//
//    pthread_create(&thread, NULL, read_handler, NULL);
//    while (1) {
//        sem_wait(sem1);
//        fgets(buffer, BUFFER_SIZE, stdin);
//
//        sem_wait(sem2);
//
//        if (strcmp(buffer, "q\n") == 0) {
//            sem_post(sem2);
//            sem_unlink(SEM1_NAME);
//            return EXIT_SUCCESS;
//        }
//
//        memcpy(msg->message, buffer, BUFFER_SIZE);
//        msg->counter++;
//
//        bzero(buffer, BUFFER_SIZE);
//        sem_post(sem2);
//    }
//
//    sem_unlink(SEM1_NAME);
//    sem_unlink(SEM2_NAME);
//
//    return EXIT_SUCCESS;
//}
