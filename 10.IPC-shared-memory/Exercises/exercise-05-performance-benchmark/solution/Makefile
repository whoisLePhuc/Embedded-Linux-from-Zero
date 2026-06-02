CC = gcc
CFLAGS = -Wall -Wextra

all: mmap_benchmark

mmap_benchmark: main.c
	$(CC) $(CFLAGS) -o mmap_benchmark main.c

clean:
	rm -f mmap_benchmark mmap_test.bin write_test.bin
