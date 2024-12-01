# Define the compiler
CC = gcc

# Define the compiler flags
CFLAGS = -Wall

# Define the target executables
SENDER_TARGET = sender.exe
RECEIVER_TARGET = receiver.exe

# Source files
SENDER_SRC = sender.c
RECEIVER_SRC = receiver.c

# Default target
all: $(SENDER_TARGET) $(RECEIVER_TARGET)

# Compile sender executable
$(SENDER_TARGET): $(SENDER_SRC)
	$(CC) $(CFLAGS) -o $@ $<

# Compile receiver executable
$(RECEIVER_TARGET): $(RECEIVER_SRC)
	$(CC) $(CFLAGS) -o $@ $<

# Clean up the build files
clean:
	rm -f $(SENDER_TARGET) $(RECEIVER_TARGET)

# Phony targets
.PHONY: all clean
