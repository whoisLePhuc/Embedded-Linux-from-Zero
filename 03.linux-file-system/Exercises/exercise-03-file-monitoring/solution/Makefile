CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = fd_monitor

all: $(TARGET)

$(TARGET): fd_monitor.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

compare: $(TARGET)
	@echo "=== Running program ==="
	./$(TARGET)
	@echo ""
	@echo "=== Compare with ls -l /proc/self/fd/ ==="
	@ls -l /proc/self/fd/

.PHONY: all clean run compare
