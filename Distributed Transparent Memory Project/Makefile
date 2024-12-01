# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = hashTable.c server.c udp_helpers.c message.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = server.exe

# Default target
all: $(EXEC)

# Rule to link object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Clean target to remove only object files
clean.o:
	rm -f $(OBJS)

# Clean target to remove only the executable
clean.exe:
	rm -f $(EXEC)

.PHONY: all clean clean.o clean.exe
