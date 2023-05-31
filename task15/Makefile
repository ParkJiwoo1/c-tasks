CC = gcc
CFLAGS = -Wall

all: client server

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server: select_server.c
	$(CC) $(CFLAGS) -o server select_server.c

clean:
	rm -f client server

