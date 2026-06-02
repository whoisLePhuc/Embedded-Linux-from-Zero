CROSS_COMPILE ?=
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
CFLAGS = -Wall -O2

TARGET = main
LIBS = libmath.a
OBJS = math_ops.o

all: $(LIBS) $(TARGET)

math_ops.o: math_ops.c math_ops.h
	$(CC) $(CFLAGS) -c -o $@ $<

libmath.a: math_ops.o
	$(AR) rcs $@ $^

main: main.c libmath.a
	$(CC) $(CFLAGS) -o $@ $< -L. -lmath

clean:
	rm -f $(OBJS) $(LIBS) $(TARGET)

.PHONY: all clean
