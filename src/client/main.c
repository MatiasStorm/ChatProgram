#include <stdlib.h>
#include "client.h"
#include "../settings.h"

int main(int argc, char const *argv[]){
    int port = argc >= 2 ? atoi(argv[1]) : PORT;
    const char *ip = argc >= 3 ? argv[2] : IP;
    run_client(port, ip);
    return 0;
}