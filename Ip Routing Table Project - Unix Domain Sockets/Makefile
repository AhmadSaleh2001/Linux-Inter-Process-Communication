# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Targets
all: client server

# Shared object
msg.o: msg.c
	$(CC) $(CFLAGS) -c msg.c -o msg.o

# Compile the client
client: client.o msg.o
	$(CC) $(CFLAGS) client.o msg.o -o client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c -o client.o

# Compile the server
server: server.o msg.o
	$(CC) $(CFLAGS) server.o msg.o -o server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -o server.o

# Clean up build files
clean:
	rm -f *.o client server
