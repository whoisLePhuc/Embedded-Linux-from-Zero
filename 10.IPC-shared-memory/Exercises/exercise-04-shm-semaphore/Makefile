CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lrt

all: producer consumer

producer: producer.c
	$(CC) $(CFLAGS) -o producer producer.c $(LDFLAGS)

consumer: consumer.c
	$(CC) $(CFLAGS) -o consumer consumer.c $(LDFLAGS)

clean:
	rm -f producer consumer
