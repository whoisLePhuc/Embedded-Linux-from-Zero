CC = gcc
CFLAGS = -g -Wall -Wextra -std=c11
TARGET = crash_me

all: $(TARGET)

$(TARGET): crash_me.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) core core.*

run: $(TARGET)
	./$(TARGET)

debug-core: $(TARGET) core
	gdb ./$(TARGET) core

.PHONY: all clean run debug-core
