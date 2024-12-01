CC = gcc
CFLAGS = -Wall
TARGETS = mx_tcp_server tcp_client
OBJECTS = mx_tcp_server.o tcp_client.o

all: $(TARGETS)

mx_tcp_server: mx_tcp_server.o
	$(CC) $(CFLAGS) -o mx_tcp_server mx_tcp_server.o

tcp_client: tcp_client.o
	$(CC) $(CFLAGS) -o tcp_client tcp_client.o

mx_tcp_server.o: mx_tcp_server.c
	$(CC) $(CFLAGS) -c mx_tcp_server.c

tcp_client.o: tcp_client.c
	$(CC) $(CFLAGS) -c tcp_client.c

clean:
	rm -f $(OBJECTS) $(TARGETS)
