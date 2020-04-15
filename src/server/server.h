#ifndef SERVER_H
#define SERVER_H
#include "../utils/llist/llist.h"

int server_on;             // Flag used to turn the server off. 
int server_sock_fd;            // The server socket file descriptor
#define MAX_CLIENT_NAME_SIZE 5 // Maximum size of the client name.

/**
 * Linked list used to store the thread data for each connected client 
 * (each client is connected by a separate thread).
*/
llist* client_llist;
pthread_mutex_t client_llist_mutex;

/**
 * Struct used to store thread specific data. Every time a new client connects a node is added
 * to the client_list, with it data equal to a connection_thread_t struct.
*/
typedef struct connection_thread_s{
    int sock_fd;
    char *client_name; 
    pthread_t *thread;
} connection_thread_t;


/**
 * @brief Start hook to the server.
 * @param[in] port The port used to listen for connections on.
*/
void run_server(int port);

/**
 * @brief Frees the connection thread struct
 * @param[in] connection Pointer to the connection thread struct.
*/
void connection_thread_destroy(void *connection);


/**
 * @brief Creates the server socket
 * @param[in] port The port to listen on.
*/
int create_server_socket(int port);


/**
 * @brief Continous loop that listens for new connections.
 * @param[in] server_sock_fd The server socket file descriptor.
*/
void listen_for_clients();


/**
 * @brief Joins all currently running threads to the calling thread.
*/
void join_connected_threads();


/**
 * @brief Sets up the connection_thread data structure and creates the new connecting thread.
*/
void initialize_client_thread();


/**
 * @brief Adds the supplied name from the client to the connection_data.
 * @param[in] sock_file The clients socket file pointer.
 * @param[in] connection_data A pointer to the threads connection_thread_t struct.
*/
void get_client_name(FILE * sock_file, connection_thread_t * connection_data);


/**
 * @brief Handles sending and recieving messages from the client.
 * @param[in]thread_data Data used by the thread should be of type connection_thread_t.
*/
void* connect_client(void* thread_data);


/**
 * @brief Broadcasts a message to all connected clients.
 * @param[in] node A pointer to the calling threads linked list node (Used to traverse the whole linked list and send the messages to all clients).
 * @param[in] message The message to send to all clients.
 * @param[in] sock_fd The file descriptor of the calling threads client.
*/
void broadcast_message(llist_node *node, char *message, int sock_fd);


/**
 * @brief Disconnects the client by closing the socket, it also free all allocated data associated with the thread.
 * @param[in] sock_file The file pointer of the socket.
 * @param[in] node Pointer to the threads linked list node.
*/
void disconnect_client(FILE* sock_file, llist_node *node);


/**
 * @brief Closes the server socket and destorys the global linked list.
*/
void cleanup();

#endif