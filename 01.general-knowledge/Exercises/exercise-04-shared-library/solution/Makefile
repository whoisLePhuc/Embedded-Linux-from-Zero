CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
LDFLAGS = -shared
TARGET = main
LIB_NAME = libmath

.PHONY: all clean test info

all: $(TARGET)

# Build shared library
$(LIB_NAME).so: math_ops.o
	$(CC) $(LDFLAGS) -o $@ $^

# Compile position-independent code
math_ops.o: math_ops.c math_ops.h
	$(CC) $(CFLAGS) -c $< -o $@

# Build main program, link with shared library
$(TARGET): main.o $(LIB_NAME).so
	$(CC) -o $@ main.o -L. -l$(LIB_NAME) -Wl,-rpath,.

main.o: main.c math_ops.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

info: $(TARGET)
	@echo "=== Program Dependencies ==="
	ldd $(TARGET)
	@echo ""
	@echo "=== RPATH ==="
	readelf -d $(TARGET) | grep RPATH
	@echo ""
	@echo "=== Library Symbols ==="
	nm -D $(LIB_NAME).so | head -20

clean:
	rm -f *.o *.so $(TARGET)
