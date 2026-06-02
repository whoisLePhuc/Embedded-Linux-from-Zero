CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = resource_usage

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
