# Chat Program
Simple chat program, that can be used to send messages between computers on a local network.

## Server
### Run
Every message that is sendt to the server is being broadcasted to all connected clients, except the sending client.
```
ChatProgra$ ./bin/server [PORT]
```
PORT defaults to 1234 if not provided.

To close the server type ```exit``` in terminal running the server.

## Client
The client can be run on the same computer as the server and on other computers on the same network.

### Run
```
ChatProgram$ ./bin/client [IP] [PORT]
```
IP is the ip-address of the computer running the server, defaults to 127.0.0.1 if not provided.
PORT defaults to 1234 if not provided, IP has to be provided if the PORT is.

When a client is connected it can send messages to all other clients connected to the server.

## Libs
### llist:
llist is a linked list implementation, used to the server to keep track of all connectec clients and broadcast messages to all connected clients.

## Make
To make the binaries run:
```
ChatProgram$ make all
```

## Test
Test the program run:
```
ChatProgram$ make test
```

## TODO:
- Improve on input args (maybe use flags?).
- Write documentation.
- Ask clients to provide a name.

