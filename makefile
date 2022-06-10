CC = gcc
CFLAGS = -g -Wall -fPIC


all: libmyfs.so libmylibc.so main main2

libmyfs.so: myfs.o
	$(CC) -shared $(CFLAGS) myfs.o -o libmyfs.so

libmylibc.so: myfs.o myfile.o
	$(CC) -shared $(CFLAGS) myfile.o -o libmylibc.so

myfs.o: myfs.c
	$(CC) $(CFLAGS) myfs.c -c

myfile.o: myfile.c
	$(CC) $(CFLAGS) myfile.c -c

main: libmyfs.so main.o
	$(CC) $(CFLAGS) main.o ./libmyfs.so -o main

main.o: main.c
	$(CC) $(CFLAGS) main.c -c

main2: libmylibc.so main2.o
	$(CC) main2.o ./libmylibc.so -o main2

main2.o: main2.c
	$(CC) main2.c -c

clean: 
	rm -f *.so *.o a.out main main2