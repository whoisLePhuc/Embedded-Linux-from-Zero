#include "sensor.h"

float read_sensor(sensor_type_t type) {
    // Simulation: read sensor values
    switch (type) {
        case TEMPERATURE:
            return 25.5f;
        case HUMIDITY:
            return 65.0f;
        case PRESSURE:
            return 1013.25f;
        default:
            return 0.0f;
    }
}

int sensor_init(void) {
    // Simulate sensor initialization
    return 0;  // Success
}

void sensor_cleanup(void) {
    // Simulate sensor cleanup
}
