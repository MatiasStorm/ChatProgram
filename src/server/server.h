#ifndef SERVER_H
#define SERVER_H
#include "../lib/llist.h"


int write_message(llist_node *node, char *buffer);
void read_message(int sock_fd, char *buffer, int buffer_size);
int disconnect_client(llist_node *socket_node, char* buffer);
void* thread_function(void* thread_data);
void cleanup();


#endif