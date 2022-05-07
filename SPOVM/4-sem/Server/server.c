//#include <stdlib.h>
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <string.h>
//#include <errno.h>
//
//int create_socket(int);
//int accept_conn(int);
//
//#ifdef WINDOWS
//#include <direct.h>
//    #define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//#define GetCurrentDir getcwd
//#endif
//
//#define MAX_TEXT_LINE   4096 /*max text line length*/
//#define MAX_CONNECTIONS 8    /*maximum number of client connections*/
//
//int main (int argc, char **argv)
//{
//    int listen_fd, connection_fd, n;
//
//    pid_t child_pid;
//    socklen_t client_socket_length;
//
//    char buf[MAX_TEXT_LINE];
//    struct sockaddr_in client_address, server_address;
//
//    // validating the input
//    if (argc != 2) {
//        fputs("Usage: ./server <port number>", stderr);
//
//        exit(1);
//    }
//
//    // Create a socket_fd for the socket
//    // If socket_fd < 0 there was an error in the creation of the socket
//    if ((listen_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
//        fputs("Problem in creating the socket", stderr);
//
//        exit(1);
//    }
//
//    // preparation of the socket address
//    server_address.sin_family = AF_INET;
//    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
//
//    if (atoi(argv[1]) <= 1024) {
//        fputs("Port number must be greater than 1024", stderr);
//
//        exit(1);
//    }
//
//    server_address.sin_port = htons(atoi(argv[1]));
//
//    // bind the socket
//    if (bind (listen_fd, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
//        fputs("Can not bind socket!", stderr);
//
//        return 1;
//    }
//
//    // listen to the socket by creating a connection queue, then wait for clients
//    if (listen(listen_fd, MAX_CONNECTIONS) < 0) {
//        fputs("listen() error!", stderr);
//
//        return 1;
//    }
//
//    puts("Server running...waiting for connections.");
//
//    while (1) {
//
//        client_socket_length = sizeof(client_address);
//        // accept a connection
//        connection_fd = accept(listen_fd, (struct sockaddr *) &client_address, &client_socket_length);
//        if (connection_fd < 0) {
//            fputs("accept() error", stderr);
//        }
//
//        puts("Received request...");
//
//        if ((child_pid = fork()) != 0) {
//            return 1;
//        }
//
//        // if it’s 0, it’s child process
//        puts("Child created for dealing with client requests");
//
//
//        //close listening socket
//        close (listen_fd);
//
//        // for data connection
//        int data_port = 1024;
//
//        while (recv(connection_fd, buf, MAX_TEXT_LINE, 0) > 0)  {
//            printf("String received from client: %s\n", buf);
//
//            char *token, *dummy;
//
//            dummy = buf;
//
//            token = strtok(dummy," ");
//
//            // If exit
//            if (strcmp("quit\n", buf) == 0) {
//                close(connection_fd);
//                puts("The client has quit");
//            }
//
//            // If ls
//            if (strcmp("ls\n", buf) == 0) {
//                FILE *in;
//
//                char temp[MAX_TEXT_LINE], port[MAX_TEXT_LINE];
//
//                int data_socket;
//                data_port = data_port + 1;
//
//                if (data_port == atoi(argv[1])) {
//                    data_port = data_port + 1;
//                }
//
//                sprintf(port,"%d", data_port);
//
//                //creating socket for data connection
//                data_socket = create_socket(data_port);
//                //sending data connection port no. to client
//                send(connection_fd, port, MAX_TEXT_LINE, 0);
//                //accepting connection from client
//                data_socket = accept_conn(data_socket);
//                if (!(in = popen("ls", "r"))) {
//                    fputs("popen() error", stderr);
//                }
//
//                while (fgets(temp, sizeof(temp), in) != NULL){
//                    send(data_socket,"1",MAX_TEXT_LINE,0);
//                    send(data_socket, temp, MAX_TEXT_LINE, 0);
//                }
//                send(data_socket,"0",MAX_TEXT_LINE,0);
//                pclose(in);
//            }
//
//            if (strcmp("pwd\n", buf) == 0) {
//                char curr_dir[MAX_TEXT_LINE];
//
//                GetCurrentDir(curr_dir,MAX_TEXT_LINE - 1);
//                send(connection_fd, curr_dir, MAX_TEXT_LINE, 0);
//            }
//
//            if (strcmp("cd", token) == 0)  {
//                token = strtok(NULL," \n");
//
//                printf("Path given is: %s\n", token);
//
//                if (chdir(token) < 0) {
//                    send(connection_fd, "0", MAX_TEXT_LINE, 0);
//                }
//                else {
//                    send(connection_fd, "1", MAX_TEXT_LINE, 0);
//                }
//            }
//
//            if (strcmp("put", token) == 0) {
//                char port[MAX_TEXT_LINE],
//                buffer[MAX_TEXT_LINE],
//                char_num_blks[MAX_TEXT_LINE],
//                char_num_last_blk[MAX_TEXT_LINE],
//                check[MAX_TEXT_LINE];
//
//                int datasock, num_blks, num_last_blk, i;
//                FILE *fp;
//
//                token = strtok(NULL," \n");
//
//                printf("Given filename is: %s\n", token);
//
//                data_port = data_port + 1;
//
//                if (data_port == atoi(argv[1])) {
//                    data_port = data_port + 1;
//                }
//
//                sprintf(port,"%d", data_port);
//                //creating socket for data connection
//                datasock = create_socket(data_port);
//                //sending data connection port to client
//                send(connection_fd, port, MAX_TEXT_LINE, 0);
//                //accepting connection
//                datasock = accept_conn(datasock);
//                recv(connection_fd, check, MAX_TEXT_LINE, 0);
//                puts(check);
//
//                if (strcmp("1", check) == 0) {
//                    if ((fp = fopen(token,"w")) == NULL)
//                        fputs("Error in creating file", stderr);
//                    else {
//                        recv(connection_fd, char_num_blks, MAX_TEXT_LINE, 0);
//                        num_blks = atoi(char_num_blks);
//                        for (i = 0; i < num_blks; i++) {
//                            recv(datasock, buffer, MAX_TEXT_LINE,0);
//                            fwrite(buffer,sizeof(char),MAX_TEXT_LINE,fp);
//                        }
//
//                        recv(connection_fd, char_num_last_blk, MAX_TEXT_LINE, 0);
//                        num_last_blk = atoi(char_num_last_blk);
//                        if (num_last_blk > 0) {
//                            recv(datasock, buffer, MAX_TEXT_LINE,0);
//                            fwrite(buffer,sizeof(char),num_last_blk,fp);
//                        }
//                        fclose(fp);
//                        puts("File downloaded");
//                    }
//                }
//            }
//
//            if (strcmp("get", token) == 0) {
//                char port[MAX_TEXT_LINE], buffer[MAX_TEXT_LINE], char_num_blks[MAX_TEXT_LINE], char_num_last_blk[MAX_TEXT_LINE];
//                int datasock, i;
//                long lSize, num_blks, num_last_blk;
//                FILE *fp;
//                token = strtok(NULL," \n");
//
//                printf("Filename given is: %s\n", token);
//                data_port = data_port + 1;
//
//                if (data_port == atoi(argv[1])){
//                    data_port = data_port + 1;
//                }
//                sprintf(port,"%d",data_port);
//                datasock = create_socket(data_port);				//creating socket for data connection
//                send(connection_fd, port, MAX_TEXT_LINE, 0);					//sending port no. to client
//                datasock = accept_conn(datasock);					//accepting connnection by client
//                if ((fp = fopen(token,"r")) != NULL) {
//                    //size of file
//                    send(connection_fd, "1", MAX_TEXT_LINE, 0);
//                    fseek (fp , 0 , SEEK_END);
//                    lSize = ftell (fp);
//                    rewind (fp);
//                    num_blks = lSize / MAX_TEXT_LINE;
//                    num_last_blk = lSize % MAX_TEXT_LINE;
//                    sprintf(char_num_blks,"%ld", num_blks);
//                    send(connection_fd, char_num_blks, MAX_TEXT_LINE, 0);
//                    //cout<<num_blks<<"	"<<num_last_blk<<endl;
//
//                    for (i = 0; i < num_blks; i++) {
//                        fread (buffer,sizeof(char),MAX_TEXT_LINE,fp);
//                        send(datasock, buffer, MAX_TEXT_LINE, 0);
//                        //cout<<buffer<<"	"<<i<<endl;
//                    }
//                    sprintf(char_num_last_blk,"%ld", num_last_blk);
//                    send(connection_fd, char_num_last_blk, MAX_TEXT_LINE, 0);
//                    if (num_last_blk > 0) {
//                        fread (buffer,sizeof(char),num_last_blk,fp);
//                        send(datasock, buffer, MAX_TEXT_LINE, 0);
//                        //cout<<buffer<<endl;
//                    }
//                    fclose(fp);
//                    puts("File uploaded");
//
//                }
//                else {
//                    send(connection_fd, "0", MAX_TEXT_LINE, 0);
//                }
//            }
//
//        }
//
//        if (n < 0)
//            fputs("Read error", stderr);
//
//        //close socket of the server
//        close(connection_fd);
//
//        exit(0);
//    }
//}
//
//int create_socket(int port)
//{
//    int listenfd;
//    struct sockaddr_in dataservaddr;
//
//
//    //Create a socket for the soclet
//    //If sockfd<0 there was an error in the creation of the socket
//    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
//        fputs("Problem in creating the data socket", stderr);
//
//        exit(2);
//    }
//
//
//    //preparation of the socket address
//    dataservaddr.sin_family = AF_INET;
//    dataservaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    dataservaddr.sin_port = htons(port);
//
//    if ((bind (listenfd, (struct sockaddr *) &dataservaddr, sizeof(dataservaddr))) == -1) {
//        fputs("Problem in binding the data socket", stderr);
//
//        exit(2);
//    }
//
//    //listen to the socket by creating a connection queue, then wait for clients
//    if (listen(listenfd, 1)) {
//        fputs("listen() error", stderr);
//    }
//
//    return (listenfd);
//}
//
//int accept_conn(int sock)
//{
//    int dataconnfd;
//    socklen_t dataclilen;
//    struct sockaddr_in datacliaddr;
//
//    dataclilen = sizeof(datacliaddr);
//    //accept a connection
//    if ((dataconnfd = accept (sock, (struct sockaddr *) &datacliaddr, &dataclilen)) == -1) {
//        fputs("Problem in accepting the data socket", stderr);
//
//        exit(2);
//    }
//
//    return(dataconnfd);
//}


#include "functions.h"

int main() {
    const char *ip = "127.0.0.1";
    const int port = 5566;

    int server_sock, client_sock;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size;
    char buffer[MESSAGE_SIZE];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (n < 0) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    if (listen(server_sock, 5) < 0) {
        perror("[-]Listening error");
        exit(1);
    }

    printf("Listening...\n");

    while (1) {

        // Ожидание подключения клиента
        address_size = sizeof(client_address);
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, &address_size);
        if (client_sock < 0) {
            printf("[-]Accepting error");
            exit(1);
        }

        printf("[+]Client connected.\n");

        // Прием ответа клиента
        while (recv(client_sock, buffer, sizeof(buffer), 0) != -1) {

            printf("%sClient:%s %s\n", COLOR_RED, COLOR_NO, buffer);

            if (strcmp(buffer, "!ls\n") == 0) {
                print_dir("/");
            }

            if (strcmp(buffer, "!pwd\n") == 0) {
                print_full_path();
            }

            // Закрытие сокета
            if (strcmp(buffer, "exit\n") == 0) {
                close_socket(client_sock);
                printf("[+]Client disconnected.\n");
            }

            // Очистка буфера
            bzero(buffer, MESSAGE_SIZE);
            rewind(stdin);
        }

    }
}
