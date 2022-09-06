#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

const char COLOR_ON[]  = "\033[1;32m";
const char COLOR_OFF[] = "\033[0m";

const int MAX_SIZE = 256;

int main(int argc, char* argv[], char* envp[]) {
  if (argc != 2) {
    fprintf(stderr, "%d is invalid arguments amount, must be 2\n", argc);
    exit(EXIT_FAILURE);
  }

  printf(COLOR_ON);

  puts("Child process data:");
  printf("Name: %s\n", argv[0]);
  printf("Pid: %d\n", getpid());
  printf("Ppid: %d\n", getppid());

  char buffer[MAX_SIZE];
  FILE* file_envp = fopen(argv[1], "r");
  if (!file_envp) {
    perror("file_envp");
    exit(errno);
  }
  while (fgets(buffer, MAX_SIZE, file_envp) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
    printf("%s=%s\n", buffer, getenv(buffer));
  }

  printf(COLOR_OFF);

  fclose(file_envp);
  exit(EXIT_SUCCESS);
}
