#ifndef SENSOR_H
#define SENSOR_H

typedef enum {
    TEMPERATURE,
    HUMIDITY,
    PRESSURE
} sensor_type_t;

float read_sensor(sensor_type_t type);
int sensor_init(void);
void sensor_cleanup(void);

#endif /* SENSOR_H */
