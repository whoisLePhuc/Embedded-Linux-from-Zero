CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread
TARGET = rwlock_benchmark

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
