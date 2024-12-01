# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Executable names
EXEC1 = mmap_as_malloc_direct_on_memory
EXEC2 = mmap_file_read_direct_on_file
EXEC3 = mmap_file_write_direct_on_file

# Source files
SRC1 = mmap_as_malloc_direct_on_memory.c
SRC2 = mmap_file_read_direct_on_file.c
SRC3 = mmap_file_write_direct_on_file.c

# Build all executables
all: $(EXEC1) $(EXEC2) $(EXEC3)

# Rule to build EXEC1
$(EXEC1): $(SRC1)
	$(CC) $(CFLAGS) -o $(EXEC1) $(SRC1)

# Rule to build EXEC2
$(EXEC2): $(SRC2)
	$(CC) $(CFLAGS) -o $(EXEC2) $(SRC2)

# Rule to build EXEC3
$(EXEC3): $(SRC3)
	$(CC) $(CFLAGS) -o $(EXEC3) $(SRC3)

# Clean up the compiled files
clean:
	rm -f $(EXEC1) $(EXEC2) $(EXEC3)
