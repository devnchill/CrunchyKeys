CC := gcc

# Directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := $(SRC_DIR)/include

# Source and Object Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Executable
TARGET := $(BUILD_DIR)/crunchykeys

# Compilation Flags
CFLAGS := -Wall -Wextra -Wpedantic -I$(INCLUDE_DIR)
LDFLAGS := -linput -ludev

# Build Rules
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cleanup
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: clean
