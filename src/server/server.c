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

#include "../settings.h"
#include "server.h"

int client_fds[MAX_CONNECTIONS];
int n_clients = 0;
int server_sock_fd;
llist* socket_list;

int write_message(llist_node *node, char *buffer){
    char message[BUF_CAP + MESSAGE_PREFIX_LENGTH];
    int sock_fd = *((int*)node->data);
    sprintf(message, "Message from %d: %s", sock_fd, buffer);
    printf("%s", message);
    llist_node *element = node;
    int* next_fd;
    while(element->next != node){
        next_fd = element->next->data;
        if(write(*next_fd, message, sizeof(message)) == -1){
            perror("write()");
            break;
        }
        element = element->next;
    }
}

void read_message(int sock_fd, char *buffer, int buffer_size){
    if(read(sock_fd, buffer, buffer_size) == -1){
        perror("read()");
    }
}

int disconnect_client(llist_node *socket_node, char* buffer){
    if(strncmp("exit", buffer, 4) == 0){
        int *socket_fd = (int*) socket_node->data;
        printf("%d left the server\n", *socket_fd);
        close(*socket_fd);
        llist_destroy_node(socket_list, &socket_node);
        return 1;
    }
    return 0;
}

void* thread_function(void* thread_data){
    llist_node *socket_node = (llist_node*) thread_data;
    int *sock_fd = (int*)socket_node->data;
    char buffer[BUF_CAP];

    for(;;){
        bzero(buffer, sizeof(buffer));
        read_message(*sock_fd, buffer, sizeof(buffer));

        if(disconnect_client(socket_node, buffer)){
            break;
        }
        write_message(socket_node, buffer);   
    }
}




void cleanup(){
    printf("Closing socket...\n");
    close(server_sock_fd);
    // Tell all threads to clean up.
    exit(0);
}


int main(int argc, char const *argv[])
{
    signal(SIGINT, cleanup);
    if (atexit(cleanup) != 0) {
        fprintf(stderr, "cannot set exit function\n");
        exit(EXIT_FAILURE);
    }
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
    server_address.sin_port = htons(PORT); 
    
    int enable = 1;
    if (setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
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

    printf("Waiting for connection...\n");

    socklen_t client_size = sizeof(client_address);


    pthread_t threads[MAX_CONNECTIONS];
    size_t n_threads = 0;
    socket_list = malloc(sizeof(llist));

    for(;;){
        int *new_socket_fd = malloc(sizeof(int));

        *new_socket_fd = accept(server_sock_fd, (struct sockaddr*) &client_address, &client_size);
            
        llist_node* socket_node = llist_add_node(socket_list, (void*)new_socket_fd);

        printf("Connection made: client_fd=%d\n", *new_socket_fd);

        pthread_create(&threads[n_threads], NULL, thread_function, (void*) socket_node);
    }

    return 0;
}
