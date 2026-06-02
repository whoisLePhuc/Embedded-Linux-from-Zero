#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * BUG 1: Off-by-one loop writes past buffer boundary
 *   for (int i = 0; i <= size; i++) should be i < size
 */
void init_buffer(char *buf, int size) {
    for (int i = 0; i <= size; i++) {
        buf[i] = 'A';
    }
}

/*
 * BUG 2: Stack buffer overflow via strcpy
 *   buffer is 16 bytes, input can be much larger
 *   strcpy should be strncpy with null termination
 */
void process_data(const char *input) {
    char buffer[16];
    strcpy(buffer, input);
    printf("Processing: %s\n", buffer);
}

/*
 * BUG 3: Null pointer dereference
 *   ptr is NULL, 100 / *ptr causes SIGFPE before reaching division
 *   Actually dereferencing NULL causes SIGSEGV
 */
void faulty_math(int *ptr) {
    int result = 100 / *ptr;
    printf("Result: %d\n", result);
}

int main(int argc, char **argv) {
    char data[] = "Hello Embedded Linux!";
    init_buffer(data, 10);
    process_data(data);

    int *null_ptr = NULL;
    faulty_math(null_ptr);

    return 0;
}
