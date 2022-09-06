#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadFunc(void* ptr) {
    char* str;
    while (1) {
        str = (char *) ptr;
//        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        pthread_mutex_lock(&mutex);
        while (*str) {
            putc_unlocked(*str, stdout);
            ++str;
            if (usleep(500000) != 0) {
                exit(1);
            }
        }
        rewind(stdout);
        pthread_mutex_unlock(&mutex);
//        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        sleep(1);
    }
}

int main() {
    int index = 0;
    char* strings[3];
    strings[0] = "Hello world\n";
    strings[1] = "Bye world!!\n";
    strings[2] = "Goodbye all\n";

    setvbuf(stdout, NULL, _IONBF, strlen(strings[0]));

    pthread_t threads[1000];
    int size = 0;

    while (1) {
        fflush(stdin);
        switch (getchar()) {
            case '+':
                printf("Current amount of threads is: %d\n", size + 1);

                pthread_create(&threads[size++], NULL,
                               threadFunc, strings[index++]);

                if (index >= 3) {
                    index = 0;
                }
                break;

            case '-':
                --size;
                pthread_cancel(threads[size]);
                pthread_join(threads[size], NULL);

                printf("Current amount of threads is: %d\n", size);
                break;

            case 'q':
                return 0;
        }
    }
}