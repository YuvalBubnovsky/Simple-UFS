CC = gcc
CFLAGS = -g -Wall -fpic

.PHONY: clean all lib

all:  main

main: main.o
	$(CC) $(CFLAGS) -o main main.o 

main.o: main.c libmyfs.so libmylibc.so
	$(CC) $(CFLAGS) -c -o main.o main.c libmyfs.so libmylibc.so

libmyfs.so: myfs.o
	(CC) -shared $(CFLAGS) -o libmyfs.so myfs.o

myfs.o: myfs.c
	$(CC) $(CFLAGS) -c -o myfs.o myfs.c

libmylibc.so: mylibc.o
	(CC) -shared $(CFLAGS) -o libmylibc.so mylibc.o

mylibc.o: myfs.c
	$(CC) $(CFLAGS) -c -o mylibc.o mylibc.c

clean:
	rm -f main  *.o *.a *.so