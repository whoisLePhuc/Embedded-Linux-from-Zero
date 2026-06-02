CC = gcc
CFLAGS = -Wall -pthread

all: producer_consumer

producer_consumer: main.c
	$(CC) $(CFLAGS) -o producer_consumer main.c

clean:
	rm -f producer_consumer
