#include <stdio.h>
#include "sensor_hal.h"

static float simulated_value = 25.0f;

int sensor_init(void) {
#ifdef TEST_MODE
    printf("[SENSOR] Init: TEST MODE - using test values\n");
    simulated_value = 42.0f;
#else
    printf("[SENSOR] Init: hardware sensor initialized\n");
#endif
    return 0;
}

float sensor_read(void) {
#ifdef TEST_MODE
    printf("[SENSOR] Read: returning test value\n");
    return simulated_value;
#else
    // Giả lập đọc sensor thực
    static int call_count = 0;
    call_count++;
    simulated_value += 0.5f;
    if (simulated_value > 50.0f) simulated_value = 20.0f;
    return simulated_value;
#endif
}

void sensor_cleanup(void) {
    printf("[SENSOR] Cleanup: sensor deinitialized\n");
}

void sensor_set_simulation(float value) {
    simulated_value = value;
}
