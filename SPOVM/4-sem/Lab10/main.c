#include <stdio.h>
#include <pthread.h>
#include <aio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#define FILES_AMOUNT 3

char *files[]  = { "../file1.txt", "../file2.txt", "../file3.txt" };
char *res_file = "../result/res.txt";

int fd[FILES_AMOUNT];
char buf[FILES_AMOUNT][BUFSIZ];

pthread_t threads[FILES_AMOUNT];

struct aiocb read_cb[FILES_AMOUNT];
struct aiocb write_cb[FILES_AMOUNT];

void * write_handler(void *arg);
void * read_handler(void *arg);

void check_read(int i) {
    int temp_fd = read_cb[i].aio_fildes;
    while (true) {
        while (aio_error(&read_cb[i]) == EINPROGRESS) {
            printf("Reading file with descriptor %d is in process...\n", temp_fd);
//            usleep(5000);
        }

        if (aio_return(&read_cb[i]) == -1) {
            printf("Error in %d file!\n", i);
            exit(1);
        } else {
            printf("Successfully read %d file!\n", i);
            puts("File contains:");

            printf("\n///////// CONTENT OF FILE %d /////////\n", i);
//            puts((char *) read_cb[i].aio_buf);

            fdatasync(temp_fd);
            close(temp_fd);

            return;
        }
    }
}

void check_write(int i) {
    int temp_fd = write_cb[i].aio_fildes;
    while (true) {
        while (aio_error(&write_cb[i]) == EINPROGRESS) {
            printf("Writing file with descriptor %d is in process...\n", temp_fd);
//            usleep(5000);
        }

        if (aio_return(&write_cb[0]) == -1) {
            printf("Error in %d file!\n", i);
            exit(1);
        } else {
            puts("Write success");

            fdatasync(temp_fd);
            close(temp_fd);

            return;
        }
    }
}

void * read_handler(void *arg) {
    int i = *(int *)arg;
//    printf("Reader thread number [%lu] started!\n", pthread_self());

    if (aio_read(&read_cb[i]) == -1) {
        perror("Reading error");
        exit(1);
    }

    check_read(i);
//    pthread_exit(NULL);
}


void * write_handler(void *arg) {
    int i = *(int *)arg;
//    printf("Writer thread number [%lu] started!\n", pthread_self());

    if (aio_write(&write_cb[i]) == -1) {
        perror("Write error!");
        exit(1);
    }

    check_write(i);
//    pthread_exit(NULL);
}


int main() {

    // Initialization
    for (int i = 0; i < FILES_AMOUNT; i++) {
        fd[i] = open(files[i], O_RDONLY);
        if (fd[i] == -1) {
            perror("Error while opening file");
            exit(1);
        }

        memset(&read_cb[i], 0, sizeof(struct aiocb));
        memset(buf[i], 0, BUFSIZ);
    }

    size_t offset = 0;

    // Processing
    for (int i = 0; i < FILES_AMOUNT; i++) {
        read_cb[i].aio_nbytes = BUFSIZ;
        read_cb[i].aio_fildes = fd[i];
        read_cb[i].aio_offset = 0;
        read_cb[i].aio_buf = buf[i];

        pthread_create(&threads[i], NULL, read_handler, &i);
        pthread_join(threads[i], NULL);

        printf("\nAfter reading: %d\n", i);

        write_cb[i].aio_nbytes = strlen(buf[i]);
        write_cb[i].aio_fildes = open(res_file, O_CREAT | O_WRONLY | O_APPEND);
        write_cb[i].aio_offset = (long)offset;
        write_cb[i].aio_buf = buf[i];

        pthread_create(&threads[i], NULL, write_handler, &i);
        pthread_join(threads[i], NULL);

        offset += strlen(buf[i]);

        printf("\nAfter writing: %d\n", i);
    }

    // Closing
    for (int i = 0; i < FILES_AMOUNT; i++) {
        close(read_cb[i].aio_fildes);
        close(write_cb[i].aio_fildes);
        printf("Descriptor %d closed!\n", read_cb[i].aio_fildes);
    }

    return 0;
}
