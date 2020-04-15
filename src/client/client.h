#if !defined(CLIENT_H)
#define CLIENT_H

/**
 * @brief Reads and prints all incomming data on a socket file descriptor to stdout.
 * @param[in] sfd Socket file descriptor to read from. 
*/
void* reader(void* sfd);

/**
 * @brief Writes all inputs from stdin to a socket file descriptor.
 * @param[in] sfd The socket file descriptor to write to.
*/
void* writer(void* sfd);

/**
 * @brief Creates a connection to the server on Port port with IP ip.
 * @param[in] port The listening port of the server.
 * @param[in] ip The IP of the server.
*/
int* connect_to_server(int port, const char *ip);


/**
 * @brief Starts the client. 
 * @param[in] port The port the server is listening on.
 * @param[in] ip The IP of the server.
*/
void run_client(int port, const char *ip);

#endif // CLIENT_H

