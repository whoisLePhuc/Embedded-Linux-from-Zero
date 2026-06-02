#ifndef SENSOR_HAL_H
#define SENSOR_HAL_H

int sensor_init(void);
float sensor_read(void);
void sensor_cleanup(void);
void sensor_set_simulation(float value);

#endif /* SENSOR_HAL_H */
