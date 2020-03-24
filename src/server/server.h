#ifndef SERVER_H
#define SERVER_H
#include "socket_node.h"

int write_message(socket_node_t *node, char *buffer);
void read_message(int sock_fd, char *buffer, int buffer_size);
int disconnect_client(socket_node_t* node, char* buffer);
void* thread_function(void* thread_data);
void cleanup();


#endif