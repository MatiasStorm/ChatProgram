#if !defined(CLIENT_H)
#define CLIENT_H

void* reader(void* thread_data);

void* writer(void* thread_data);

int* connect_to_server(int port, const char *ip);

void run_client(int port, const char *ip);

struct pollfd* create_poll_fd(int fd);

#endif // CLIENT_H

