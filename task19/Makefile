CC = gcc
CFLAGS = -Wall

all: client print

client: client.c
	$(CC) $(CFLAGS) -o client client.c

task19: print.c
	$(CC) $(CFLAGS) -o print print.c

clean:
	rm -f print client

