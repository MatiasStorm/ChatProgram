typedef struct _llist_node{
    void *data;
    struct _llist_node *next;
} llist_node;


typedef struct _llist{
    struct _llist_node *head;
    void (*free_data_func)(void*);
} llist;

llist* llist_create(void(*free_data_func)(void*));
llist_node* llist_add_node(llist* list, void* data);
void llist_destroy_node(llist* list, llist_node** node);
void llist_destroy(llist** list);