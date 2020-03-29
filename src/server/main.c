#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "../settings.h"


int main(int argc, char const *argv[])
{
    int port = argc >= 2 ? atoi(argv[1]) : PORT;
    run_server(port);
    return 0;
}