#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *duplicate_message(const char *msg) {
    size_t len = strlen(msg);
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    strcpy(copy, msg);
    return copy;
}

char *to_uppercase(const char *msg) {
    size_t len = strlen(msg);
    char *upper = malloc(len + 1);
    if (!upper) return NULL;
    for (size_t i = 0; i < len; i++) {
        char c = msg[i];
        if (c >= 'a' && c <= 'z')
            upper[i] = c - 32;
        else
            upper[i] = c;
    }
    upper[len] = '\0';
    return upper;
}

void process_message(const char *msg) {
    char *copy = duplicate_message(msg);
    if (!copy) return;
    printf("Message: %s\n", copy);

    char *upper = to_uppercase(copy);
    printf("Processed: %s\n", upper);

    free(copy);
    free(upper);
}

int main() {
    char *buffer = malloc(10);
    if (!buffer) return 1;
    for (int i = 0; i < 10; i++) {
        buffer[i] = 'A' + i;
    }
    buffer[9] = '\0';

    process_message(buffer);

    free(buffer);
    return 0;
}
