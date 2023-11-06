# List of source files in your project
SOURCES := $(wildcard *.c)

# List of object files generated from source files
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))

# The final library to be created
LIBRARY := libmylib.a

# Compiler and compilation flags
CC := gcc
# Compiler flags
CFLAGS = -Wall -fPIC -I../include

# Default target: build the library
all: $(LIBRARY)

# Rule to compile source files into object files
archive_lib/%.o: archive_lib/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the library from object files
$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

# Clean rule: remove object files and the library
clean:
	rm -f $(OBJECTS) $(LIBRARY)
