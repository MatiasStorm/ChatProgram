#include <stdlib.h>
#include "client.h"
#include "../settings.h"

int main(int argc, char const *argv[]){
    const char *ip = argc >= 2 ? argv[2] : IP;
    int port = argc >= 3 ? atoi(argv[1]) : PORT;
    run_client(port, ip);
    return 0;
}