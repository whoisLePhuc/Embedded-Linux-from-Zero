CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = linked_list

all: $(TARGET)

$(TARGET): linked_list.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
