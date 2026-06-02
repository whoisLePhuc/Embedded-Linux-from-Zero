CC = gcc
CFLAGS = -g -Wall -Wextra -std=c11
TARGET = crash_debug

all: $(TARGET)

$(TARGET): crash_debug.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean run debug
