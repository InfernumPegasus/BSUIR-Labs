#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdbool.h>

#define CAPACITY 10

typedef struct message {
    char string[255];
} message_t;

struct Circle {
    message_t message[CAPACITY];
    int added;
    int deleted;
    int head;
    int tail;
};
