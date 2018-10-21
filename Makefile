CC=gcc

all: server client

client: client.c
	$(CC) -o client client.c

server: server.c
	$(CC) -o server server.c
