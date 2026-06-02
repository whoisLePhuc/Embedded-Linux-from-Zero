CC = gcc
CFLAGS = -Wall -pthread

all: thread_args

thread_args: main.c
	$(CC) $(CFLAGS) -o thread_args main.c

clean:
	rm -f thread_args
