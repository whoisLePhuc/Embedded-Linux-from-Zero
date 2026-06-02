CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = main

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) output.log

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
