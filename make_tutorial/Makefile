# Provide the compiler and its flags
CC = gcc
CFLAGS = -Wall

# Provide Directories and Files
SRC_DIR = .			# Source code directory
BUILD_DIR = build_dir		# Build Directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)		#Find all source files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))	# Create object file List
TARGET = myprogram # Output the binary name

#default Target
all: $(BUILD_DIR) $(TARGET)

# Create the Build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the Target binaries
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ_FILES)

# Compile each source file into object file 
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)


# Declare the targets that are not associated with files
.PHONY: all clean