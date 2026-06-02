CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: dining_philosophers

dining_philosophers: main.c
	$(CC) $(CFLAGS) -o dining_philosophers main.c

clean:
	rm -f dining_philosophers
