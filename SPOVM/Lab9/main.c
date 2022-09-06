#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define KEY 0x15

#define BUFFER_SIZE 128

typedef struct message_struct {
    char message[BUFFER_SIZE];
    int current_index;
} message_t;

typedef struct counter_struct {
    int read_counter;
    int process_counter;
} count_t;


static char *rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

message_t *message_struct;
count_t *data_count_struct;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

_Noreturn void * read_handler(void *arg) {
    int temp_index = 0;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (message_struct->current_index != temp_index) {

            printf("Read message: %s\n", message_struct->message);
            temp_index = message_struct->current_index;
            data_count_struct->read_counter++;
            printf("Local counter: %d\n", data_count_struct->read_counter);
            printf("Process counter: %d\n\n", data_count_struct->process_counter);
        }

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condition);
    }
}

int main() {
    int shared_memory_id;

    int fd = open("./shared_memory",O_RDWR);

    message_struct = mmap(NULL,
                          sizeof(message_t),
                          PROT_WRITE | PROT_READ,
                          MAP_SHARED,
                          fd,
                          0);

    if (message_struct == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    message_struct->current_index = 0;
    close(fd);

    if ((shared_memory_id = shmget(KEY, sizeof(int), 0644 | IPC_CREAT)) == -1) {
        perror("shmget error");
        exit(1);
    }

    if ((data_count_struct = shmat(shared_memory_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        exit(1);
    }

    if (data_count_struct->process_counter == 1) {
        message_struct->current_index = 0;
        bzero(message_struct->message, BUFFER_SIZE);
    }

//    message_struct->current_index = 0;
//    bzero(message_struct->message, BUFFER_SIZE);
//    sleep(10);

    pthread_t listener;
    pthread_create(&listener, NULL, read_handler, NULL);

    pthread_mutex_lock(&mutex);
    data_count_struct->process_counter++;
    pthread_mutex_unlock(&mutex);

    char *to_write = (char *) malloc(BUFFER_SIZE * sizeof(char));
    for (;;) {

        puts("Enter data to write:");
        fgets(to_write, BUFFER_SIZE, stdin);
        to_write[strlen(to_write) - 1] = 0;
        if (strlen(to_write) >= BUFFER_SIZE - 2)
            while ((getchar()) != '\n');
        rewind(stdin);

//        puts("String generated randomly!");
//        to_write = rand_string(to_write, BUFFER_SIZE / 2);

        if (strcmp(to_write, "exit") == 0) {
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&condition);

            data_count_struct->process_counter--;
            if (data_count_struct->process_counter < 1) {
                data_count_struct->process_counter = 0;
            }

            exit(EXIT_SUCCESS);
        }
        printf("Writing: %s\n", to_write);

        pthread_mutex_lock(&mutex);
        if (data_count_struct->read_counter < data_count_struct->process_counter) {
            pthread_cond_wait(&condition, &mutex);
        }

        strcpy(message_struct->message, to_write);
        message_struct->current_index++;
        data_count_struct->read_counter = 0;
        pthread_mutex_unlock(&mutex);
    }

    data_count_struct->process_counter--;
    if (data_count_struct->process_counter < 1) {
        data_count_struct->process_counter = 0;
    }

    return EXIT_SUCCESS;
}
