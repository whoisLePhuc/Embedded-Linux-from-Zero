CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = process_info

all: $(TARGET)

$(TARGET): process_info.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
