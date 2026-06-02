#include <stdio.h>
#include "calculator.h"
#include "logger.h"
#include "sensor.h"
#include "display.h"

int main() {
    log_info("Application started");

    int result = calculate(10, 5, OP_ADD);
    log_info("10 + 5 = %d", result);

    float temp = read_sensor(TEMPERATURE);
    log_info("Temperature: %.1f C", temp);

    display_show("System Ready");
    log_info("Application completed");

    return 0;
}
