# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -fPIC -I../include

# Source files
SOURCES := $(wildcard *.c)

# Object files
# List of object files generated from source files
OBJS := $(patsubst %.c, %.o, $(SOURCES))

# Library name and version
LIBNAME = mylib
VERSION = 1

# Shared library file name
SHARED_LIB = lib$(LIBNAME).so.$(VERSION)

# Build shared library
$(SHARED_LIB): $(OBJS)
	$(CC) -shared -Wl,-soname,lib$(LIBNAME).so.$(VERSION) -o $(SHARED_LIB) $(OBJS)
	ln -s lib$(LIBNAME).so.$(VERSION) lib$(LIBNAME).so

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(SHARED_LIB)

# Install library
install: $(SHARED_LIB)
	# Install the shared library to /usr/local/lib
	cp $(SHARED_LIB) /usr/local/lib
	ldconfig

# Uninstall library
uninstall:
	# Remove the shared library from /usr/local/lib
	rm -f /usr/local/lib/$(SHARED_LIB)
	ldconfig

.PHONY: clean install uninstall
