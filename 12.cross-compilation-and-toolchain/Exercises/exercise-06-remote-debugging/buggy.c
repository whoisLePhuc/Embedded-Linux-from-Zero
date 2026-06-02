#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_buffer(char *buf, int size) {
    for (int i = 0; i <= size; i++) {
        buf[i] = 'A';
    }
}

void process_data(const char *input) {
    char buffer[16];
    strcpy(buffer, input);
    printf("Processing: %s\n", buffer);
}

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
