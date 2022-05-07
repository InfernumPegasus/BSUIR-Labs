#include "functions.h"

void get_string(char* string, int len) {
    fgets(string, len, stdin);

    if (strlen(string) == len - 1)
        while ((getchar()) != '\n');
}

void print_help() {
    printf("Commands:\n"
           "help - print list of commands\n"
           "ls   - print files in client dir\n"
           "!ls  - print files in server dir\n"
           "pwd  - print full client path\n"
           "!pwd - print full server path\n"
           "send - send file\n"
           //           "get  - get file\n"
           "exit - close client\n\n");
}

// simple dir_walk implementation
void print_dir(char *path) {

    if ( strcmp(path, "") == 0 ) {
        path = malloc(FILENAME_SIZE);
        path = getcwd(path, FILENAME_SIZE);
        printf("Empty path provided! Showing info in '%s':\n", path);
    }

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    while ( (entry = readdir(dir)) ) {
        char *dirName = entry->d_name;

        if (entry->d_type == DT_DIR && (strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0)) {
            continue;
        }

        printf("%s\n", entry->d_name);

    }
}

void print_full_path() {
    char *path = malloc(FILENAME_SIZE);
    path = getcwd(path, FILENAME_SIZE);
    printf("Path: %s\n", path);
}

void close_socket(int socket_fd) {
    if (close(socket_fd)) {
        fputs("[-]Error closing socket", stderr);
        exit(EXIT_FAILURE);
    }
}