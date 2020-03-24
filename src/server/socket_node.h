#ifndef SOCKET_NODE_H
#define SOCKET_NODE_H

typedef struct socket_node_s socket_node_t;
struct socket_node_s{
    int sock_fd;
    socket_node_t *next;
};


void destroy_socket_node(socket_node_t *node, socket_node_t *head);
void push_socket_node(socket_node_t *node, socket_node_t *head);

#endif
