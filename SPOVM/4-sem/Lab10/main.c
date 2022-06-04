#include <stdio.h>
#include <pthread.h>
#include <aio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>

char *files[] = {"../file1.txt", "../file2.txt"};
char *result_file = "../result/res.txt";
char *buf1;
char *buf2;


//int files_counter;

int read_index = 0;
int write_index = 0;

struct aiocb *reader_cb, *writer_cb;

bool can_read = true, can_write = false;

int array_length() {
    int i;
    for (i = 0; files[i] != NULL; i++);
    return i - 1;
}

void set_can_write(int sig) {
    (void)sig;
    can_write = true;
}

void set_can_read(int sig) {
    (void)sig;
    can_read = true;
}

void print_array() {
    puts("Files found:");
    for (int i = 0; i < files_counter; i++) {
        puts(files[i]);
    }
}

void* writer_handler(void *a) {
    (void)a;

    long offset = 0;

    int flags = O_CREAT | O_RDWR | O_APPEND;

    while (files_counter > 0) {
        while (can_write == false) {}

        can_write = false;

        writer_cb = malloc(sizeof(struct aiocb));

        if ((writer_cb->aio_fildes = open(result_file, flags)) == -1) {
            perror("Open result file error: ");
            exit(EXIT_FAILURE);
        }

        writer_cb->aio_nbytes = strlen(buf1);
        writer_cb->aio_buf = buf1;
        writer_cb->aio_offset = offset;
        writer_cb->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        writer_cb->aio_sigevent.sigev_signo  = SIGUSR2;
        offset += (long)strlen(buf1);

        if (aio_write(writer_cb) == -1) {
            perror("Aio write error: ");
            exit(EXIT_FAILURE);
        }

        files_counter--;
    }

    return NULL;
}

/*
 * TODO подготовка структуры к чтению / записи пока другое считывется / записывается
 */

void* read_handler(void* a) {
    (void)a;

    for (int i = 0; i < files_counter; i++) {
        while (can_read == false) {}

        can_read = false;

        reader_cb = malloc(sizeof(struct aiocb));

        buf1 = malloc(256);

        reader_cb->aio_nbytes = 256;
        reader_cb->aio_offset = 0;
        reader_cb->aio_buf = buf1;
        reader_cb->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        reader_cb->aio_sigevent.sigev_signo  = SIGUSR1;

        if ((reader_cb->aio_fildes = open(files[i], O_RDONLY)) == -1) {
            perror("Open result file error");
            exit(EXIT_FAILURE);
        }

        if (aio_read(reader_cb) == -1) {
            perror("Aio read error");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        puts("Usage: <dir_path>");
        exit(1);
    }

    files_counter = array_length();
    char *dir_name = argv[1];

    print_array();
    printf("Count: %d\n", files_counter);
    printf("Dir name: %s\n", dir_name);
    printf("Res file: %s\n", result_file);

    pthread_t writer, reader;

    signal(SIGUSR1, set_can_write);
    signal(SIGUSR2, set_can_read);

    pthread_create(&writer, NULL, &writer_handler, NULL);
    pthread_create(&reader, NULL, &read_handler, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    return 0;
}