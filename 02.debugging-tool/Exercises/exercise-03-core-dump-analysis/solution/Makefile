CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = crash_me

all: $(TARGET)

$(TARGET): crash_me.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) core core.*

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
