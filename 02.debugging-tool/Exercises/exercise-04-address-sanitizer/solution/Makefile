CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
ASAN_FLAGS = -fsanitize=address -O1
TARGET = memory_ops

all: asan

asan: memory_ops.c
	$(CC) $(CFLAGS) $(ASAN_FLAGS) -o $(TARGET) $^

plain: memory_ops.c
	$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all asan plain clean run
