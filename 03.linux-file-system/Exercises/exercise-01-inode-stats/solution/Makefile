CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = file_info

all: $(TARGET)

$(TARGET): file_info.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) ../test_data/sample.txt

.PHONY: all clean run
