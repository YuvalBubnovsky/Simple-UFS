CC = gcc
CFLAGS = -g -Wall -fpic

.PHONY: clean all

all:  main libmyfs.so

main: main.o
	$(CC) $(CFLAGS) -o main main.o 

main.o: main.c libmyfs.so 
	$(CC) $(CFLAGS) -c -o main.o main.c libmyfs.o

libmyfs.so: myfs.o
	(CC) -shared $(CFLAGS) -o libmyfs.so myfs.o

myfs.o: myfs.c
	$(CC) $(CFLAGS) -c myfs.c -o myfs.o

clean:
	rm -f main  *.o *.a *.so