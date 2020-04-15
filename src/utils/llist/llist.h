typedef struct _llist_node{
    void *data;
    struct _llist_node *next;
} llist_node;


typedef struct _llist{
    struct _llist_node *head;
    void (*free_data_func)(void*);
} llist;


/**
 * @brief Creats a new linked list.
 * @param[in] free_data_func Method that will be called to free the data in each node should have the form free_data_func(void *data).
 * @return A pointer to a linked list struct.
 * 
*/
llist* llist_create(void(*free_data_func)(void*));

/**
 * @brief Adds a new node the the linked list.
 * @param[in] list The list to add the node to.
 * @param[in] data The data to be stored in the node.
 * @return A pointer to the newly created node.
*/
llist_node* llist_add_node(llist* list, void* data);

/**
 * @brief Removes a node from the linked list and calls the free_data_func, to free the data within the node.
 * @param[in] list The linked list.
 * @param[in] node A pointer to the pointer of the node.
*/
void llist_destroy_node(llist* list, llist_node** node);

/**
 * @brief Destorys the linked list and frees all the nodes and their data.
 * @param[in] list A pointer to the pointer of the linked list.
*/
void llist_destroy(llist** list);


