#!/bin/sh
BASEDIR=$(pwd)

gcc $BASEDIR/src/server/server.c ./src/server/socket_node.c -lpthread -o $BASEDIR/bin/server

gcc $BASEDIR/src/server/socket_node.c -o $BASEDIR/bin/socket_node

gcc $BASEDIR/src/client.c -lpthread -o $BASEDIR/bin/client
