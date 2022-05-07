#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define COLOR_RED "\033[1;31;40m"
#define CLEAR_CLS "\033[1;0m"

#define ALARM_DELAY 10000

int isAllowed; // 0 - restricted, 1 - allowed
int alarmSigCounter = 0;

struct statistics {
    int counter_O_O;
    int counter_I_O;
    int counter_O_I;
    int counter_I_I;
} stats;

struct mystruct {
    int firstNumber;
    int secondNumber;
} numbers;

void showStats(void) {
    if(isAllowed == 0) {
        return;
    }

    printf("\n%sChild process info\n", COLOR_RED);
    printf("Process id: %d\n", getpid());
    printf("Parent process id: %d\n", getppid());
    printf("Stats: %d %d %d %d %s\n",
           stats.counter_O_O,
           stats.counter_O_I,
           stats.counter_I_O,
           stats.counter_I_I,
           CLEAR_CLS);
}

int checkNumbers(void) {
    if(numbers.firstNumber == 0 && numbers.secondNumber == 0) {
        numbers.secondNumber = 1;
        return 1;
    }

    if(numbers.firstNumber == 0 && numbers.secondNumber == 1) {
        numbers.secondNumber = 0;
        numbers.firstNumber = 1;
        return 2;
    }

    if(numbers.firstNumber == 1 && numbers.secondNumber == 0) {
        numbers.secondNumber = 1;
        return 3;
    }

    if(numbers.firstNumber == 1 && numbers.secondNumber == 1) {
        numbers.secondNumber = 0;
        numbers.firstNumber = 0;
        return 4;
    }

    return 0;
}

void restrictOutput(int sig) {
    isAllowed = 0;
    return;
}

void allowOutput(int sig) {
    isAllowed = 1;
    return;
}

void timerHandler(int sig) {
    (void)sig;

    switch (checkNumbers()) {
        case 1:
            stats.counter_O_O++;
            break;
        case 2:
            stats.counter_O_I++;
            break;
        case 3:
            stats.counter_I_O++;
            break;
        case 4:
            stats.counter_I_I++;
            break;
        default:
            return;
    }

    alarmSigCounter++;
    if(alarmSigCounter == 101) {
        showStats();
        alarmSigCounter = 0;
    }

    return;
}

int main() {
    struct sigaction first, second, third;
    struct itimerval timer;

    first.sa_handler  = restrictOutput;
    second.sa_handler = allowOutput;
    third.sa_handler  = timerHandler;

    sigaction(SIGUSR1, &first, NULL);
    sigaction(SIGUSR2, &second, NULL);
    sigaction(SIGVTALRM, &third, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = ALARM_DELAY;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = ALARM_DELAY;

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL)) {
        perror("setitimer");
        exit(errno);
    }

    isAllowed = 1;
    stats.counter_O_I = 0;
    stats.counter_O_O = 0;
    stats.counter_I_I = 0;
    stats.counter_I_O = 0;

    numbers.firstNumber = 0;
    numbers.secondNumber = 0;

    int buf;
    while(1) {
        buf = checkNumbers();
    }
}