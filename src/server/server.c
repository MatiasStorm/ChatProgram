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
#include <fcntl.h>

#include "../settings.h"
#include "../utils/io.h"
#include "server.h"

int server_sock_fd;
int server_on = 1;

llist* client_llist;

typedef struct client_thread_s{
    int sock_fd;
    pthread_t *thread;
} connection_thread_t;

void connection_thread_destroy(void *connection){
    connection_thread_t* connection_thread = (connection_thread_t*) connection;
    free(connection_thread->thread);
    free(connection_thread);
}

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
    llist_destroy(&client_llist);
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
    
    client_llist = llist_create(connection_thread_destroy);

    int error_code;
    while(server_on){
        poll_ret = poll(poll_fds, 2, timeout);
        if(poll_ret == -1){
            perror("poll()");
            exit(0);
        }
        else if(poll_fds[0].revents){
            initialize_client_thread();
        }
        else if(poll_fds[1].revents){
            char buf[10];
            bzero(buf, 10);
            fgets(buf, 10, stdin);
            if(strncmp("exit", buf, 4) == 0){
                server_on = 0;
                break;
            }
        }
    }
    join_connected_threads();
}

void join_connected_threads(){
    llist_node *client_node = client_llist->head;
    if(client_node != NULL){
        pthread_t *thread;
        do{
            printf("Joining thread %d\n", ((connection_thread_t*)(client_node->data))->sock_fd);
            thread = ((connection_thread_t*)(client_node->data))->thread;
            pthread_join(*thread, NULL);

            client_node = client_node->next;
        } while(client_node != client_llist->head);
    }
}


void initialize_client_thread(){
    struct sockaddr_in client_address; 
    socklen_t client_size = sizeof(client_address);

    connection_thread_t *thread_data = malloc(sizeof(connection_thread_t));
    thread_data->thread = malloc(sizeof(pthread_t));

    int client_sock_fd = accept(server_sock_fd, (struct sockaddr*) &client_address, &client_size);
    thread_data->sock_fd = client_sock_fd;
    llist_node* socket_node = llist_add_node(client_llist, (void*)thread_data);
    
    pthread_t *thread = ((connection_thread_t*) socket_node->data)->thread;
    int error_code;
    if((error_code = pthread_create(thread, NULL, connect_client, (void*) socket_node)) != 0){
        printf("pthread_create Error: %d\n", error_code);
        exit(0);
    }
    printf("Connection made: client_fd=%d\n", client_sock_fd);
}


void* connect_client(void* thread_data){
    llist_node* node = (llist_node*) thread_data;
    connection_thread_t *connection_data = (connection_thread_t*) (node->data);
    int sock_fd = connection_data->sock_fd;
    FILE *sock_file = fdopen(sock_fd, "r+");

    int timeout, poll_ret;
    struct pollfd poll_fds[1];

    poll_fds[0].fd = sock_fd;
    poll_fds[0].events = POLLIN;
    timeout = 1000;

    char *message;
    while(server_on){
        poll_ret = poll(poll_fds, 1, timeout);
        if(poll_ret == -1){
            perror("poll()");
            exit(0);
        }
        else if(poll_fds[0].revents){
            message = read_from_file(sock_file);
            if(strncmp("exit", message, 4) == 0){
                break;
            }
            broadcast_message(node, message, sock_fd);   
            free(message);
            message = NULL;
        }
    }
    free(message);
    message = NULL;

    disconnect_client(sock_file, node);
    fclose(sock_file);
}


int broadcast_message(llist_node *node, char *message, int sock_fd){
    char broadcast_message[20 + strlen(message)];
    sprintf(broadcast_message, "Message from %d: %s", sock_fd, message);
    printf("%s", broadcast_message);
    
    llist_node *element = node;
    int next_fd;
    while(element->next != node){
        next_fd = ((connection_thread_t*)(element->next->data))->sock_fd;
        FILE *next_file = fdopen(next_fd, "w");
        write_to_file(next_file, broadcast_message);
        fflush(next_file);
        element = element->next;
    }
}

void disconnect_client(FILE* sock_file, llist_node *node){
    connection_thread_t *connection_data = (connection_thread_t*) (node->data);
    int sock_fd = connection_data->sock_fd;
    if(!server_on){ // Tell client to exit.
        printf("Writing exit to client: %d\n", sock_fd);
        write_to_file(sock_file, "exit\n");
        fflush(sock_file);
        sleep(2); // Give client time to disconnect
    }
    else {
        int error_code;
        if( (error_code = pthread_detach(*(connection_data->thread))) != 0){ // Detach client thread
            printf("pthread_detach Error: %d\n", error_code);
            exit(0);            
        }
        llist_destroy_node(client_llist, &node);
    }

    if(fcntl(sock_fd, F_GETFD) != -1){
        close(sock_fd);
    }    
    printf("%d left the server\n", sock_fd);
}


