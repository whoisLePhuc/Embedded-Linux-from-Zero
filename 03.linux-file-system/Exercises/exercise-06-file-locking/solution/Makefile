CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGETS = account_manager range_locker

all: $(TARGETS)

account_manager: account_manager.c
	$(CC) $(CFLAGS) -o $@ $^

range_locker: range_locker.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) account.txt records.dat

test-account:
	@echo "=== Testing account_manager ==="
	@rm -f account.txt
	@echo "1000" > account.txt
	@./account_manager balance
	@./account_manager deposit 500
	@./account_manager withdraw 200
	@./account_manager balance

test-range:
	@echo "=== Testing range_locker ==="
	@./range_locker init
	@./range_locker lock 0 "Test Data A"
	@./range_locker lock 1 "Test Data B"
	@./range_locker show

test: all test-account test-range

.PHONY: all clean test test-account test-range
