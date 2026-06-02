CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: processA processB

processA: processA.c
	$(CC) $(CFLAGS) -o processA processA.c

processB: processB.c
	$(CC) $(CFLAGS) -o processB processB.c

clean:
	rm -f processA processB
