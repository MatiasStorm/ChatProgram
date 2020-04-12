#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "llist.h"


llist* llist_create(void(*free_data_func)(void*)){
    llist* list = malloc(sizeof(llist));
    list->head = NULL;
    list->free_data_func = free_data_func;
    return list;
}

llist_node* llist_add_node(llist* list, void* data){
    llist_node *node = malloc(sizeof(llist_node));
    node->data = data;
    if(list->head == NULL){
        list->head = node;
        node->next = node;
    }
    else{
        node->next = list->head->next;
        list->head->next = node;
        list->head = node;
    }
    return node;
}

void llist_destroy_node(llist* list, llist_node** node){
    if(list == NULL || node == NULL || list->head == NULL){
        return;
    }
    llist_node *n = list->head;
    if(list->head == *node){
        while(n->next != list->head){
            n = n->next;
        }
        // Only one node
        if(n->next == n){
            list->head = NULL;
        } 
        else{
            n->next = list->head->next;
            list->head = n;
        }
    }
    else {
        while(n->next != *node){
            n = n->next;
        }
        n->next = n->next->next;
    }
    list->free_data_func((*node)->data);
    (*node)->data = NULL;
    free(*node);
    *node = NULL;
}

void llist_destroy(llist** list){
    llist_node* node;
    while((*list)->head != NULL){
        node = (*list)->head;
        llist_destroy_node(*list, &node);
    }
    free(*list);
    *list = NULL;
}