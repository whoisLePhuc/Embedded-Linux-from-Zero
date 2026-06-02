#include <stdio.h>

int main() {
    printf("Hello from Embedded Linux!\n");
    printf("Architecture: ");
    #ifdef __arm__
        printf("ARM\n");
    #elif defined(__aarch64__)
        printf("ARM64\n");
    #elif defined(__x86_64__)
        printf("x86_64\n");
    #elif defined(__i386__)
        printf("x86\n");
    #else
        printf("Unknown\n");
    #endif
    return 0;
}
