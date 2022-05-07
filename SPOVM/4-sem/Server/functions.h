#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

#define MESSAGE_SIZE  1024
#define FILENAME_SIZE 512

#define COLOR_RED "\033[1;31;40m"
#define COLOR_NO  "\033[1;0m"

void get_string(char* string, int len);

void print_help();

void print_full_path();

void print_dir(char *path);

void close_socket(int socket_fd);

//void send_file(FILE *file, int socket_fd) {
//    char data[MESSAGE_SIZE] = {0};
//
//    while ( fgets(data, MESSAGE_SIZE, file) != NULL ) {
//        if (send(socket_fd, data, sizeof(data), 0)== -1) {
//            perror("[-]Error in sending data");
//            exit(1);
//        }
//        bzero(data, MESSAGE_SIZE);
//    }
//    fclose(file);
//}
//
//void send_file_menu(int socket_fd) {
//    char filename[FILENAME_SIZE];
//    puts("Enter filepath:");
//    get_string(filename, FILENAME_SIZE);
//    printf("Entered filepath: %s\n", filename);
//
//    FILE *file;
//    if ( (file = fopen(filename, "r")) == NULL ) {
//        puts("[-]Error while opening file");
//        exit(1);
//    }
//
//    send_file(file, socket_fd);
//}

#endif //SERVER_FUNCTIONS_H
