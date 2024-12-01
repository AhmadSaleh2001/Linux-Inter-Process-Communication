# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Linker flags
LDFLAGS = -lrt

# Target executables
TARGETS = recvr.exe sender.exe

# Default rule to build both executables
all: $(TARGETS)

# Rule to build recvr.exe
recvr.exe: recvr.o
	$(CC) -o $@ recvr.o $(LDFLAGS)

# Rule to build sender.exe
sender.exe: sender.o
	$(CC) -o $@ sender.o $(LDFLAGS)

# Rule to compile recvr.c into object file
recvr.o: recvr.c
	$(CC) $(CFLAGS) -c recvr.c

# Rule to compile sender.c into object file
sender.o: sender.c
	$(CC) $(CFLAGS) -c sender.c

# Clean up the build
clean:
	rm -f *.o $(TARGETS)
