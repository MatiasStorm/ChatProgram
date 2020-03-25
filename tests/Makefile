CFLAGS = -Wall
LIBS = -lcheck -lm -lpthread -lsubunit -lrt

test: llist_test.c
	gcc -o llist_test llist_test.c ../src/lib/llist.c $(LIBS)
	./llist_test

# test: socket_node_test.o socket_node.o
# 	gcc -o test socket_node_test.o socket_node.o  $(LIBS)
# 	./test

# socket_node_test.o: socket_node_test.c
# 	gcc $(CFLAGS) -c socket_node_test.c

# socket_node.o: ../src/server/socket_node.c
# 	gcc $(CFLAGS) -c ../src/server/socket_node.c

clean:
	rm llist_test
