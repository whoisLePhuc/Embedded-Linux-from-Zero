CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: thread_race

thread_race: main.c
	$(CC) $(CFLAGS) -o thread_race main.c

clean:
	rm -f thread_race
