#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <wait.h>
#include <pthread.h>
#include <poll.h>

#include "../settings.h"
#include "../utils/io.h"
#include "client.h"

int client_on = 1;

void run_client(int port, const char* ip){
    int* sock_fd = connect_to_server(port, ip);

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, writer, (void*) sock_fd);
    pthread_create(&threads[1], NULL, reader, (void*) sock_fd);
    for(int i = 0; i < 2; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("pthread_join()");
            exit(1);
        }
    }
    if(close(*sock_fd) == -1){
        perror("close()");
        exit(0);
    }
    free(sock_fd);
    printf("Exiting...\n");
}

int* connect_to_server(int port, const char *ip){
    int* sock_fd = malloc(sizeof(int));
    struct sockaddr_in server_address;
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(*sock_fd == -1){
        printf("socket creation failed...\n");
        exit(1);
    }
    printf("Socket created, fd = %d!\n", *sock_fd);
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    printf("Connecting to server on, %s:%d\n", ip, port);
    if(connect(*sock_fd, (struct sockaddr*) &server_address, sizeof(server_address)) != 0){
        printf("Connection to server failed...\n");
        exit(1);
    }
    printf("connected to server, sock_fd: %d\n", *sock_fd);
    return sock_fd;
}

void* reader(void* thread_data){
    int sock_fd = *((int*) thread_data);

    struct pollfd poll_fd[1];
    poll_fd[0].fd = sock_fd;
    poll_fd[0].events = POLLIN;
    int poll_ret;
    int timeout = 1000;

    FILE *sock_file = fdopen(sock_fd, "r");
    char *message;

    while(client_on){
        poll_ret = poll(poll_fd, 1, timeout);
        if(poll_ret == -1){
            perror("poll()");
            exit(0);
        }
        else if(poll_fd->revents & POLLIN && client_on){
            message = read_from_file(sock_file);
            if(strncmp("exit", message, 4) == 0){
                printf("Server closed.\n");
                client_on = 0;
            }
            free(message);
            message = NULL;
        }
    }
}

void* writer(void* thread_data){
    int sock_fd = *((int*) thread_data);
    
    struct pollfd poll_fd[1];
    poll_fd[0].fd = STDIN_FILENO;
    poll_fd[0].events = POLLIN;
    int poll_ret;
    int timeout = 1000;
    
    FILE *sock_file = fdopen(sock_fd, "w");
    char *input;

    while(client_on){
        poll_ret = poll(poll_fd, 1, timeout);
        if(poll_ret == -1){
            perror("poll()");
            exit(0);
        }
        else if(poll_fd->revents & POLLIN){
            input = read_from_file(stdin);
            if(strncmp("exit", input, 4) == 0){
                client_on = 0;
            }
            write_to_file(sock_file, input); // Has to be after setting client_on = 0, to avoid race condition.
            
            free(input);
            input = NULL;
        }
    }
}

