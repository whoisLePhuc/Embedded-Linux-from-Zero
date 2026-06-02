#include "logger.h"
#include <stdio.h>
#include <time.h>

static const char *log_level_strings[] = {
    "INFO",
    "WARNING",
    "ERROR"
};

void log_init(void) {
    printf("=== Logger Initialized ===\n");
}

void log_message(LogLevel level, const char *message) {
    time_t now;
    struct tm *timeinfo;
    char time_buffer[80];
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    printf("[%s] [%s] %s\n", time_buffer, log_level_strings[level], message);
}
