CC = gcc
CFLAGS = -Wall
TARGETS = server.exe client_af_unix.exe client_queue.exe
OBJECTS = server.o client_af_unix.o client_queue.o

all: $(TARGETS)

server.exe: server.o
	$(CC) $(CFLAGS) -o server.exe server.o

client_af_unix.exe: client_af_unix.o
	$(CC) $(CFLAGS) -o client_af_unix.exe client_af_unix.o

client_queue.exe: client_queue.o
	$(CC) $(CFLAGS) -o client_queue.exe client_queue.o

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client_af_unix.o: client_af_unix.c
	$(CC) $(CFLAGS) -c client_af_unix.c

client_queue.o: client_queue.c
	$(CC) $(CFLAGS) -c client_queue.c

clean:
	rm -f $(OBJECTS) $(TARGETS)
