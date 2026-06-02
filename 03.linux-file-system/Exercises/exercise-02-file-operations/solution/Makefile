CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = file_copy

all: $(TARGET)

$(TARGET): file_copy.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) output.txt

test: $(TARGET)
	./$(TARGET) ../test_data/input.txt output.txt
	@echo "Checking if files are identical..."
	@diff ../test_data/input.txt output.txt && echo "✓ Files match!" || echo "✗ Files differ!"

run: $(TARGET)
	./$(TARGET) ../test_data/input.txt output.txt

.PHONY: all clean test run
