CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = crash_debug

all: $(TARGET)

$(TARGET): crash_debug.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
