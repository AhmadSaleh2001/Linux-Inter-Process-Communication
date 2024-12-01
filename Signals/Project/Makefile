# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Executable names
EXEC_CLIENT = client
EXEC_SERVER = server

# Object files
OBJ_CLIENT = client.o
OBJ_IPADDRESS = ipAddress.o
OBJ_MAC = mac.o
OBJ_SERVER = server.o

# Source files
SRC_CLIENT = client.c
SRC_IPADDRESS = ipAddress.c
SRC_MAC = mac.c
SRC_SERVER = server.c

# Build all executables in the specified order
all: $(EXEC_CLIENT) $(EXEC_SERVER)

# Rule to build client executable
$(EXEC_CLIENT): $(OBJ_IPADDRESS) $(OBJ_MAC) $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o $(EXEC_CLIENT) $(OBJ_CLIENT) $(OBJ_IPADDRESS) $(OBJ_MAC)

# Rule to build server executable
$(EXEC_SERVER): $(OBJ_IPADDRESS) $(OBJ_MAC) $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o $(EXEC_SERVER) $(OBJ_SERVER) $(OBJ_IPADDRESS) $(OBJ_MAC)

# Rule to compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the compiled files
clean:
	rm -f $(EXEC_CLIENT) $(EXEC_SERVER) *.o
