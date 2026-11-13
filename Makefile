CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic
SOURCE := TEXT EDITOR/main.c
BUILD_DIR := build
TARGET := $(BUILD_DIR)/text-editor

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) "$(SOURCE)" -o "$(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
