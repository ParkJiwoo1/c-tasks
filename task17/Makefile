CC = gcc
CFLAGS = -Wall

all: client server

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server: pthread_server.c
	$(CC) $(CFLAGS) -o server pthread_server.c linklist.c -lpthread

clean:
	rm -f client server

