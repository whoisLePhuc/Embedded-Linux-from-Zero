CC = gcc
CFLAGS = -Wall -pthread

all: race_condition

race_condition: main.c
	$(CC) $(CFLAGS) -o race_condition main.c

clean:
	rm -f race_condition
