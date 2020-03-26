#if !defined(CLIENT_H)
#define CLIENT_H

void* reader(void* thread_data);
void* writer(void* thread_data);
int* connect_to_server();
void run_client();

#endif // CLIENT_H

