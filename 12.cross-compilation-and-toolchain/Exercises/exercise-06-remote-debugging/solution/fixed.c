#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_buffer(char *buf, int size) {
    for (int i = 0; i < size; i++) {
        buf[i] = 'A';
    }
}

void process_data(const char *input) {
    char buffer[32];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    printf("Processing: %s\n", buffer);
}

void faulty_math(int *ptr) {
    if (ptr == NULL) {
        printf("Error: null pointer passed to faulty_math\n");
        return;
    }
    int result = 100 / *ptr;
    printf("Result: %d\n", result);
}

int main(int argc, char **argv) {
    char data[] = "Hello Embedded Linux!";
    init_buffer(data, 10);
    process_data(data);

    int value = 5;
    faulty_math(&value);

    return 0;
}
