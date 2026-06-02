CC = gcc
CFLAGS = -g -Wall -Wextra -std=c11
ASAN_FLAGS = -fsanitize=address -g -O1
TARGET = memory_ops

all: $(TARGET)

$(TARGET): memory_ops.c
	$(CC) $(CFLAGS) -o $@ $^

asan: memory_ops.c
	$(CC) $(CFLAGS) $(ASAN_FLAGS) -o $(TARGET) $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

run-asan: asan
	./$(TARGET)

.PHONY: all asan clean run run-asan
