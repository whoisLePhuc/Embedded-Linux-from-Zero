CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt

all: writer reader

writer: writer.c
	$(CC) $(CFLAGS) -o writer writer.c $(LDFLAGS)

reader: reader.c
	$(CC) $(CFLAGS) -o reader reader.c $(LDFLAGS)

clean:
	rm -f writer reader
