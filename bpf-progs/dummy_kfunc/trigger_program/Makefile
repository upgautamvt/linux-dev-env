# Makefile for building test_unlink

# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -O2

# Target executable
TARGET  := trigger

# Source files
SRCS    := trigger.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
