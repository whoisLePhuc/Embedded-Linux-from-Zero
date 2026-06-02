CC = gcc
CFLAGS = -g -Wall -Wextra -std=c11
TARGET = linked_list

all: $(TARGET)

$(TARGET): linked_list.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean run debug
