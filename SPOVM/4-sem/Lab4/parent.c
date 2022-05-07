#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void printPids(int **pids, int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        printf("%d ", (*pids)[i]);
    }
    puts("\n");
}

char* EnterLineReturn(void) {
    char *line;
    int lineSize = 0, letterNum = 0;
    char letter;
    line = (char*)malloc(1*sizeof(char));

    while(1) {
        scanf("%c", &letter);

        if(letter == '\0') {
            continue;
        } else if (letter == '\n') {
            line[letterNum] = '\0';
            break;
        }

        line[letterNum] = letter;
        lineSize+=2;
        letterNum++;
        line = (char*)realloc(line, lineSize);
    }

    return line;
}

//returns number that located int the end of line or 0 if no number where found
int ParseIntFromEnd(char *line) {
    //find end of line
    int endLine = 0;

    for(int i = 0; line[i] != '\0';i++)
        endLine = i;

    int number = 0; //converting;
    int pos = 0, temp;

    for(int i = endLine; i > 0; i--) {
        if((line[i] >= '0' && line[i] <='9') || line[i] == '-') {
            if(line[i] == '-') {
                number *= -1;
                return number;
            }

            temp = line[i] - 48;
            for(int j = 0;j < pos; j++)
                temp *= 10;
            pos++;
            number+=temp;
        } else
            break;
    }

    return number;
}

void addToArray(int pid, int **pids, int arraySize) {
    int *temp;
    temp = (int*)realloc(*pids, (arraySize + 1) * sizeof(int));
    *pids = temp;

    if(!*pids) {
        printf("Allocation Error - %d\n", errno);
        exit(errno);
    }

    (*pids)[arraySize] = pid;

    printf("\nChild process created\n");
    printf("Amount of processes: %d\n\n", arraySize + 1);
}

void deleteLastChild(int **pids, int arraySize) {
    int *temp;
    int isKilled;
    if(arraySize == 0) {
        printf("No child processes\n");
        return;
    }

    int cpid = (*pids)[arraySize - 1];

    isKilled = kill(cpid, SIGKILL);

    wait(&cpid);
    printf("%d - process was killed\n",cpid);

    if(arraySize == 1) {
        free(*pids);
        temp = (int*)malloc(sizeof(int));
    } else
        temp = (int*)realloc(*pids, (arraySize - 1) * sizeof(int));

    *pids = temp;

    printf("\nChild process deleted\n");
    printf("Amount of processes: %d\n", arraySize - 1);
}

void deleteAllChild(int **pids, int arraySize) {
    int isKilled;
    if(arraySize == 0 || !*pids) {
        printf("\nNo child processes\n");
        return;
    }

    for(int i = 0; i < arraySize; i++) {
        isKilled = kill((*pids)[i], SIGKILL);
        if(isKilled != 0)
            printf("Error killing process! Try to exit program to prevent 'child' process stuck in system\n");
        else{
            printf("%d - process was killed\n",(*pids)[i]);
            wait(&(*pids)[i]);
        }
    }

    free(*pids);

    printf("\nAll child processes deleted\n");
}

int isAlarm;
void myAlarm(int sig) {
    (void)sig;
    isAlarm = 1;
    return;
}

void myHandler(int sig) {
    (void)sig;
    return;
}


int main() {
    char *option;
    pid_t pid;

    isAlarm = 0;

    int arraysize = 0;
    int *pids = (int*)malloc(sizeof(int));
    int pidNumber;

    struct sigaction mysignal, noKillParent;
    mysignal.sa_handler = myAlarm;
    noKillParent.sa_handler = myHandler;
    sigaction(SIGALRM, &mysignal, 0);
    sigaction(SIGUSR2, &noKillParent, 0);

    while(1) {
        rewind(stdin);
        printf("> ");
        option = EnterLineReturn();
        if(option[0] == '\0')
            continue;
        switch (option[0]) {
            case '+':
                pid = fork();

                if(pid == -1) {
                    printf("Fork: Error - %d\n", errno);
                    exit(errno);
                }

                if(pid != 0) {
                    addToArray(pid, &pids, arraysize);
                    arraysize++;
                } else {
                    if(execl("./child", "child", NULL) == -1) {
                        printf("Execl: Error - %d\n", errno);
                        exit(errno);
                    }
                }
                break;
            case '-':
                deleteLastChild(&pids, arraysize);
                arraysize--;
                break;
            case 'k':
                deleteAllChild(&pids, arraysize);
                arraysize = 0;
                pids = (int*)malloc(sizeof(int));
                break;
            case 'g':
                pidNumber = ParseIntFromEnd(option);

                if(pidNumber < 0 || pidNumber >= arraysize) {
                    printf("No such pid\n");
                    break;
                }

                if(pidNumber == 0) {
                    for(int i = 0; i < arraysize; i++)
                        kill(pids[i], SIGUSR2);
                }
                else {
                    kill(pids[pidNumber - 1], SIGUSR2);
                }
                break;
            case 's':
                pidNumber = ParseIntFromEnd(option);

                if(pidNumber < 0 || pidNumber >= arraysize) {
                    printf("No such pid\n");
                    break;
                }

                if(pidNumber == 0) {
                    for(int i = 0; i < arraysize; i++)
                        kill(pids[i], SIGUSR1);
                }
                else {
                    kill(pids[pidNumber - 1], SIGUSR1);
                }
                break;
            case 'p':
                pidNumber = ParseIntFromEnd(option);

                if(pidNumber < 0 || pidNumber >= arraysize) {
                    printf("No such pid\n");
                    break;
                }

                if(pidNumber == 0) {
                    for(int i = 0; i < arraysize; i++)
                        kill(pids[i], SIGUSR1);
                }
                else {
                    for(int i = 0; i < arraysize; i++) {
                        if(i == pidNumber)
                            continue;
                        kill(pids[pidNumber], SIGUSR1);
                    }
                }

                alarm(5);
                sleep(5);

                if (isAlarm == 1) {
                    for (int i = 0; i < arraysize; i++)
                        kill(pids[i], SIGUSR2);
                    isAlarm = 0;
                }

                break;
            case 'q':
                deleteAllChild(&pids, arraysize);
                exit(0);
        }
    }
}