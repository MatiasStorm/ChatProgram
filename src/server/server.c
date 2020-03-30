#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>

#include "../settings.h"
#include "../utils/io.h"
#include "server.h"

int client_fds[MAX_CONNECTIONS];
int n_clients = 0;
int server_sock_fd;
int server_on = 1;
llist* socket_list;

void run_server(int port){
    signal(SIGINT, cleanup);
    if (atexit(cleanup) != 0) {
        fprintf(stderr, "cannot set exit function\n");
        exit(EXIT_FAILURE);
    }

    int server_sock_fd = create_server_socket(port);
    listen_for_clients(server_sock_fd);
}

void cleanup(){
    printf("Closing socket...\n");
    close(server_sock_fd);
    llist_destroy(&socket_list);
    // Tell all threads to clean up.
    exit(0);
}

int create_server_socket(int port){
    struct sockaddr_in server_address, client_address; 
  
    // socket create and verification 
    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_sock_fd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    printf("Socket successfully created..\n"); 
    bzero(&server_address, sizeof(server_address)); 
  
    // assign IP, PORT 
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_address.sin_port = htons(port); 
    
    int enable = 1;
    if (setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(0);
    }

    // Binding newly created socket to given IP and verification 
    if ((bind(server_sock_fd, (struct sockaddr_in*) &server_address, sizeof(server_address))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(server_sock_fd, MAX_CONNECTIONS)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 

    printf("Listening on file descriptor %d, port %d\n", server_sock_fd, ntohs(server_address.sin_port));
    return server_sock_fd;
}

void listen_for_clients(int server_sock_fd){
    printf("Waiting for connection...\n");

    int timeout, poll_ret;
    struct pollfd poll_fds[2];

    poll_fds[0].fd = server_sock_fd;
    poll_fds[0].events = POLLIN;
    poll_fds[1].fd = STDIN_FILENO;
    poll_fds[1].events = POLLIN;
    timeout = 5000;    
    
    struct sockaddr_in client_address; 
    socklen_t client_size = sizeof(client_address);

    pthread_t threads[MAX_CONNECTIONS];
    size_t n_threads = 0;
    socket_list = llist_create();

    while(server_on){
        poll_ret = poll(poll_fds, 2, timeout);
        if(poll_ret == -1){
            perror("poll()");
            exit(0);
        }
        else if(poll_fds[0].revents){
            int *new_socket_fd = malloc(sizeof(int));

            *new_socket_fd = accept(server_sock_fd, (struct sockaddr*) &client_address, &client_size);
                
            llist_node* socket_node = llist_add_node(socket_list, (void*)new_socket_fd);

            printf("Connection made: client_fd=%d\n", *new_socket_fd);

            pthread_create(&threads[n_threads], NULL, connect_client, (void*) socket_node);
        }
        else if(poll_fds[1].revents){
            char buf[10];
            bzero(buf, 10);
            fgets(buf, 10, stdin);
            if(strncmp("exit", buf, 4) == 0){
                exit(0);
            }
        }
    }
}

void* connect_client(void* thread_data){
    llist_node *socket_node = (llist_node*) thread_data;
    int *sock_fd = (int*)socket_node->data;
    char buffer[BUF_CAP];
    FILE *sock_file = fdopen(*sock_fd, "rw");
    char *message;
    while(server_on){
        bzero(buffer, sizeof(buffer));
        message = read_from_file(sock_file);

        if(strncmp("exit", message, 4) == 0){
            break;
        }
        broadcast_message(socket_node, message);   
        free(message);
    }
    free(message);
    disconnect_client(socket_node);
    fclose(sock_file);
}


int broadcast_message(llist_node *node, char *message){
    int sock_fd = *((int*)node->data);
    printf("Message from %d: %s", sock_fd, message);
    
    llist_node *element = node;
    int* next_fd;
    while(element->next != node){
        next_fd = element->next->data;
        FILE *next_file = fdopen(*next_fd, "w");
        write_to_file(next_file, message);
        element = element->next;
    }
}

void disconnect_client(llist_node *socket_node){
    int *socket_fd = (int*) socket_node->data;
    printf("%d left the server\n", *socket_fd);
    close(*socket_fd);
    llist_destroy_node(socket_list, &socket_node);
}


