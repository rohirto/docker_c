# Provide the compiler and its flags
CC = gcc
CFLAGS = -Wall

# Provide Directories and Files
SRC_DIR = .			# Source code directory
BUILD_DIR = build_dir		# Build Directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)		#Find all source files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))	# Create object file List
TARGET = myprogram

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean