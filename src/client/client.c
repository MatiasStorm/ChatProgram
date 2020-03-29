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

#include "../settings.h"
#include "client.h"

int client_on = 1;

void run_client(int port, const char* ip){
    int* sock_fd = connect_to_server(port, ip);

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, writer, (void*) sock_fd);
    pthread_create(&threads[1], NULL, reader, (void*) sock_fd);
    pthread_join(threads[0], NULL);

    close(*sock_fd);
    free(sock_fd);
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
    char buffer[BUF_CAP + MESSAGE_PREFIX_LENGTH];
    int sock_fd = *((int*) thread_data);
    while(client_on){
        bzero(buffer, sizeof(buffer));
        read(sock_fd, buffer, sizeof(buffer));
        printf("READ: %s", buffer);
    }
}

void* writer(void* thread_data){
    char buffer[BUF_CAP];
    int sock_fd = *((int*) thread_data);
    int n;
    while(client_on){
        bzero(buffer, sizeof(buffer));
        n = 0;
        while((buffer[n++] = getchar()) != '\n');
        
        write(sock_fd, buffer, sizeof(buffer));

        if(strncmp("exit", buffer, 4) == 0){
            printf("Client closed. Exiting.\n");
            client_on = -1;
            break;
        }
    }
}

