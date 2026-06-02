CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = simple_shell

all: $(TARGET)

$(TARGET): simple_shell.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
