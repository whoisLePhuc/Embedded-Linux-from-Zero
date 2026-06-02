#include <stdio.h>
#include "display.h"

int display_init(void) {
    printf("[DISPLAY] Initialized\n");
    return 0;
}

void display_show(const char *message) {
    printf("[DISPLAY] %s\n", message);
}

void display_clear(void) {
    printf("[DISPLAY] Screen cleared\n");
}
