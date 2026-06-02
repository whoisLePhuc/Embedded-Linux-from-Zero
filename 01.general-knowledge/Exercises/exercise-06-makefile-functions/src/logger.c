#include <stdio.h>
#include <time.h>
#include "logger.h"

static log_level_t current_level = LOG_INFO;

void log_set_level(log_level_t level) {
    current_level = level;
}

void log_message(log_level_t level, const char *file, int line,
                 const char *fmt, ...) {
    if (level > current_level) return;

    const char *levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timebuf[20];
    strftime(timebuf, sizeof(timebuf), "%H:%M:%S", tm_info);

    printf("[%s] [%s] %s:%d: ", timebuf, levels[level], file, line);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
