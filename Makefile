CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC -Iinclude
LDFLAGS = -ldl

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = test

LIB_SRC = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRC))


STATIC_LIB = $(BUILD_DIR)/libpkcs11_crypto.a
SHARED_LIB = $(BUILD_DIR)/libpkcs11_crypto.so
TEST_APP = $(BUILD_DIR)/example_app

all: $(STATIC_LIB) $(SHARED_LIB)

# Build static library
$(STATIC_LIB): $(LIB_OBJS)
	ar rcs $@ $^

# Build shared library
$(SHARED_LIB): $(LIB_OBJS)
	$(CC) -shared -o $@ $^

# Build test application
$(TEST_APP): $(TEST_DIR)/example_app.c $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(BUILD_DIR)/libpkcs11_crypto.a $(LDFLAGS)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
