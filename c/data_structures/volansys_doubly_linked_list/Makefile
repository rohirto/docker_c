SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -Iinc -g

# Get a list of all .c files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Derive the object file names from the source file names
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# The final executable name
TARGET = $(BUILD_DIR)/a.out

all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	$(CC)  $^ -o $@ $(CFLAGS)
# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC)  -c $< -o $@ $(CFLAGS)
# Clean rule
clean:
	rm -rf $(BUILD_DIR)

# Ensure that build directory exists before building
$(shell mkdir -p $(BUILD_DIR))
