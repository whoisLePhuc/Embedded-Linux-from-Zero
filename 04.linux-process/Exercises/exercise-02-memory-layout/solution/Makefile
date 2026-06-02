CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = memory_layout

all: $(TARGET)

$(TARGET): memory_layout.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
