#ifndef SERVER_H
#define SERVER_H
#include "../../lib/llist.h"


void run_server();

int create_server_socket();

void listen_for_clients(int server_sock_fd);

void* connect_client(void* thread_data);

int write_message(llist_node *node, char *buffer);

void read_message(int sock_fd, char *buffer, int buffer_size);

int disconnect_client(llist_node *socket_node, char* buffer);

void cleanup();

#endif