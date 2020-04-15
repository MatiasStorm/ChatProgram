#ifndef SERVER_H
#define SERVER_H
#include "../utils/llist/llist.h"

typedef struct connection_thread_s{
    int sock_fd;
    char *client_name; 
    pthread_t *thread;
} connection_thread_t;

void run_server(int port);

void connection_thread_destroy(void *connection);

int create_server_socket(int port);

void listen_for_clients(int server_sock_fd);

void join_connected_threads();

void initialize_client_thread();

void get_client_name(FILE * sock_file, connection_thread_t * connection_data);

void* connect_client(void* thread_data);

int broadcast_message(llist_node *node, char *buffer, int sock_fd);

void read_message(int sock_fd, char *buffer, int buffer_size);

void disconnect_client(FILE* sock_file, llist_node *node);

void cleanup();

#endif