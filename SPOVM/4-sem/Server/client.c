//#include <stdlib.h>
//#include <stdio.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <string.h>
//#include <arpa/inet.h>
//#include <errno.h>
//
//int create_socket(int,char *);
//
//#ifdef WINDOWS
//#include <direct.h>
//    #define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//
//#define GetCurrentDir getcwd
//#endif
//
//#define MAX_TEXT_LINE 4096 /*max text line length*/
//
///*
// * fd - File Descriptor
// * sd - Socket File Descriptor (как пример, fd который связан с сокетом)
// */
//
//
//int main(int argc, char **argv)
//{
//    int socket_fd;
//    struct sockaddr_in server_address;
//    char send_request[MAX_TEXT_LINE];
//
//    // basic check of the arguments
//    // additional checks can be inserted
//    if (argc != 3) {
//        fputs("Usage: ./client <IP address of the server> <port number>", stderr);
//
//        exit(1);
//    }
//
//    // Create a socket for the client
//    // If socket_fd < 0 there was an error in the creation of the socket
//    if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
//        fputs("Problem in creating the socket", stderr);
//
//        exit(1);
//    }
//
//    // Creation of the socket
//    memset(&server_address, 0, sizeof(server_address));
//    server_address.sin_family = AF_INET;
//    server_address.sin_addr.s_addr= inet_addr(argv[1]);
//    server_address.sin_port =  htons(atoi(argv[2])); // convert to big-endian order
//
//    //Connection of the client to the socket
//    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
//        fputs("Problem in connecting to the server", stderr);
//
//        exit(errno);
//    }
//
//    printf("ftp>");
//
//    while (fgets(send_request, MAX_TEXT_LINE, stdin) != NULL) {
//
//        send(socket_fd, send_request, MAX_TEXT_LINE, 0);
//        char *token,*dummy;
//        dummy=send_request;
//        token=strtok(dummy," ");
//
//        if (strcmp("quit\n", send_request) == 0)  {
//            close(socket_fd);
//            return 0;
//        }
//
//        else if (strcmp("ls\n", send_request) == 0)  {
//            char buff[MAX_TEXT_LINE],check[MAX_TEXT_LINE]="1",port[MAX_TEXT_LINE];
//            int data_port,datasock;
//            recv(socket_fd, port, MAX_TEXT_LINE, 0);				//reciening data connection port
//            data_port=atoi(port);
//            datasock=create_socket(data_port,argv[1]);
//            while(strcmp("1",check)==0){ 				//to indicate that more blocks are coming
//                recv(datasock, check, MAX_TEXT_LINE, 0);
//                if(strcmp("0",check)==0)			//no more blocks of data
//                    break;
//                recv(datasock, buff, MAX_TEXT_LINE, 0);
//                puts(buff);
//            }
//
//        }
//
//        else if (strcmp("!ls\n", send_request) == 0)  {
//            system("ls");
//        }
//
//        else if (strcmp("pwd\n", send_request) == 0) {
//            char curr_dir[MAX_TEXT_LINE];
//            recv(socket_fd, curr_dir, MAX_TEXT_LINE, 0);
//            puts(curr_dir);
//        }
//
//        else if (strcmp("!pwd\n", send_request) == 0)  {
//            system("pwd");
//        }
//
//        else if (strcmp("cd", token) == 0)  {
//            char check[MAX_TEXT_LINE];
//            token=strtok(NULL," \n");
//            printf("Path given is: %s\n", token);
//
//            recv(socket_fd, check, MAX_TEXT_LINE, 0);
//            if(strcmp("0",check) == 0){
//                fputs("Directory doesn't exist. Check Path", stderr);
//            }
//
//        }
//
//        else if (strcmp("!cd", token) == 0)  {
//            token=strtok(NULL," \n");
//            printf("Path given is: %s\n", token);
//
//            if (chdir(token) < 0) {
//                fputs("Directory doesn't exist. Check Path", stderr);
//            }
//        }
//
//        else if (strcmp("put",token)==0)  {
//            char port[MAX_TEXT_LINE], buffer[MAX_TEXT_LINE],char_num_blks[MAX_TEXT_LINE],char_num_last_blk[MAX_TEXT_LINE];
//            int data_port,datasock,i;
//            long lSize, num_blks, num_last_blk;
//            FILE *fp;
//            recv(socket_fd, port, MAX_TEXT_LINE, 0);				//receiving the data port
//            data_port = atoi(port);
//            datasock=create_socket(data_port,argv[1]);
//            token=strtok(NULL," \n");
//            if ((fp=fopen(token,"r"))!=NULL)
//            {
//                //size of file
//                send(socket_fd, "1", MAX_TEXT_LINE, 0);
//                fseek (fp , 0 , SEEK_END);
//                lSize = ftell (fp);
//                rewind (fp);
//                num_blks = lSize / MAX_TEXT_LINE;
//                num_last_blk = lSize % MAX_TEXT_LINE;
//                sprintf(char_num_blks,"%ld",num_blks);
//                send(socket_fd, char_num_blks, MAX_TEXT_LINE, 0);
//                //cout<<num_blks<<"	"<<num_last_blk<<endl;
//
//                for(i= 0; i < num_blks; i++) {
//                    fread (buffer, sizeof(char), MAX_TEXT_LINE, fp);
//                    send(datasock, buffer, MAX_TEXT_LINE, 0);
//                    //cout<<buffer<<"	"<<i<<endl;
//                }
//                sprintf(char_num_last_blk,"%ld", num_last_blk);
//                send(socket_fd, char_num_last_blk, MAX_TEXT_LINE, 0);
//                if (num_last_blk > 0) {
//                    fread (buffer,sizeof(char),num_last_blk,fp);
//                    send(datasock, buffer, MAX_TEXT_LINE, 0);
//                    //cout<<buffer<<endl;
//                }
//                fclose(fp);
//                puts("File uploaded");
//            }
//            else {
//                send(socket_fd, "0", MAX_TEXT_LINE, 0);
//                fputs("Error in opening file. Check filename\nUsage: put filename", stderr);
//            }
//        }
//
//        else if (strcmp("get",token)==0) {
//            char port[MAX_TEXT_LINE], buffer[MAX_TEXT_LINE],char_num_blks[MAX_TEXT_LINE],char_num_last_blk[MAX_TEXT_LINE],message[MAX_TEXT_LINE];
//            int data_port, datasock, num_blks, num_last_blk, i;
//            FILE *fp;
//            recv(socket_fd, port, MAX_TEXT_LINE, 0);
//            data_port=atoi(port);
//            datasock=create_socket(data_port,argv[1]);
//            token=strtok(NULL," \n");
//            recv(socket_fd, message, MAX_TEXT_LINE, 0);
//            if(strcmp("1",message)==0){
//                if((fp=fopen(token,"w")) == NULL)
//                    fputs("Error in creating file", stderr);
//                else
//                {
//                    recv(socket_fd, char_num_blks, MAX_TEXT_LINE, 0);
//                    num_blks=atoi(char_num_blks);
//                    for(i = 0; i < num_blks; i++) {
//                        recv(datasock, buffer, MAX_TEXT_LINE, 0);
//                        fwrite(buffer, sizeof(char), MAX_TEXT_LINE, fp);
//                        //cout<<buffer<<endl;
//                    }
//                    recv(socket_fd, char_num_last_blk, MAX_TEXT_LINE, 0);
//                    num_last_blk = atoi(char_num_last_blk);
//                    if (num_last_blk > 0) {
//                        recv(datasock, buffer, MAX_TEXT_LINE, 0);
//                        fwrite(buffer,sizeof(char),num_last_blk,fp);
//                        //cout<<buffer<<endl;
//                    }
//                    fclose(fp);
//                    puts("File downloaded");
//                }
//            }
//            else {
//                fputs("Error in opening file. Check filename\nUsage: put filename", stderr);
//            }
//        }
//        else {
//            puts("Error in command. Check Command");
//        }
//        printf("ftp>");
//
//    }
//
//    exit(0);
//}
//
//
//int create_socket(int port,char *address)
//{
//    int socket_fd;
//    struct sockaddr_in server_address;
//
//
//    //Create a socket for the client
//    //If socket_fd<0 there was an error in the creation of the socket
//    if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
//        fputs("Problem in creating the socket", stderr);
//
//        exit(1);
//    }
//
//    //Creation of the socket
//    memset(&server_address, 0, sizeof(server_address));
//    server_address.sin_family = AF_INET;
//    server_address.sin_addr.s_addr= inet_addr(address);
//    server_address.sin_port =  htons(port); //convert to big-endian order
//
//    //Connection of the client to the socket
//    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
//        fputs("Problem in creating data channel", stderr);
//
//        exit(3);
//    }
//
//    return socket_fd;
//}



#include "functions.h"

int main() {
    char *ip = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in address;
    char buffer[MESSAGE_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = port;
    address.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("[-]Connection error");
        exit(1);
    }

    printf("Connected to the server.\n");

    while (1) {
        bzero(buffer, MESSAGE_SIZE);
        printf("Enter command: ");
        get_string(buffer, MESSAGE_SIZE);
        printf("Client: %s\n", buffer);
        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "help\n") == 0) {
            print_help();
        }

        // TODO
        if (strcmp(buffer, "ls\n") == 0) {
            print_dir("");
        }

        if (strcmp(buffer, "pwd\n") == 0) {
            print_full_path();
        }

        // TODO отправить файл
        if (strcmp(buffer, "send\n") == 0) {
//            send_file_menu(sock);
        }

        if (strcmp(buffer, "exit\n") == 0) {
            // ЗАКРЫТИЕ СОКЕТА
            close(sock);
            printf("Disconnected from the server.\n");
            return 0;
        }
    }

//    // ОТПРАВКА СООБЩЕНИЯ СЕРВЕРУ
//    bzero(buffer, MESSAGE_SIZE);
//    printf("Enter command: ");
//    get_string(buffer, MESSAGE_SIZE);
//    printf("Client: %s\n", buffer);
//    send(sock, buffer, strlen(buffer), 0);
//
//
//    // ПОЛУЧЕНИЕ ОТВЕТА ОТ СЕРВЕРА
//    bzero(buffer, MESSAGE_SIZE);
//    recv(sock, buffer, sizeof(buffer), 0);
//    printf("Server: %s\n", buffer);
}
