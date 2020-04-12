CFLAGS = -Wall
LIBS = -lcheck -lm -lpthread -lsubunit -lrt

test: llist_test.c
	gcc -o llist_test llist_test.c ../src/utils/llist/llist.c $(LIBS)
	./llist_test
	
clean:
	rm llist_test
