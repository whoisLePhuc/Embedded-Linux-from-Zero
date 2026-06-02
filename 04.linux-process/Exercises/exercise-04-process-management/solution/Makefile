CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGETS = zombie_demo process_pool

all: $(TARGETS)

zombie_demo: zombie_demo.c
	$(CC) $(CFLAGS) -o $@ $^

process_pool: process_pool.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)

run-zombie: zombie_demo
	./zombie_demo

run-pool: process_pool
	./process_pool

.PHONY: all clean run-zombie run-pool
