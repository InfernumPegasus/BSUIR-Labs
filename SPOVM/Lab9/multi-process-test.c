#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>

int main() {

#define ITERATIONS 3

    pid_t pid;

    for (int i = 0; i < ITERATIONS; i++) {
        printf("Execl Iteration %d\n", i);

        pid = fork();

        if (pid > 0) {
            // is parent
            puts("PARENT PROCESS!");
        } else if(pid == 0) {
            // is child
            execl("writer", NULL);
        } else {
            // fork didn't work
            puts("FORK DO NOT WORK");
            exit(1);
        }
    }

    for (int i = 0; i < ITERATIONS; i++) {
        wait(NULL);
    }

    return 0;
}
