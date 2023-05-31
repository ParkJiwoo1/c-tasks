CC = gcc
CFLAGS = -Wall

all: client server

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server: fork_server.c
	$(CC) $(CFLAGS) -o server fork_server.c

clean:
	rm -f client server

