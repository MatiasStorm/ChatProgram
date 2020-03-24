#include "socket_node.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



void destroy_socket_node(socket_node_t *node, socket_node_t *head){
    printf("Destroying %p\n", node);
    close(node->sock_fd);
    socket_node_t *element = head;
    if(node->sock_fd == head->sock_fd){
        while(element->next != head){
            element = element->next;
        }
        if(element->next == head){
            head = NULL;
        }
        else {
            element->next = head->next;
            head = element;
        }
    }
    else{
        while(node->sock_fd != element->next->sock_fd){
            element = element->next;
        }
        element->next = element->next->next;
    }
    free(node);
}

void push_socket_node(socket_node_t *node, socket_node_t *head){
        if(head == NULL){
            node->next = node;
        }
        else{
            node->next = head->next;
            head->next = node;
        }
        head = node;
}
