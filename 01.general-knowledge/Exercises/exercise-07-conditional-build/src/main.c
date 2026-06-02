#include <stdio.h>
#include "sensor_hal.h"
#include "platform.h"

#ifdef DEBUG
    #define DEFAULT_SAMPLE_INTERVAL 1
#else
    #define DEFAULT_SAMPLE_INTERVAL 5
#endif

int main() {
    printf("====================================\n");
    printf("  Embedded Sensor Application\n");
    printf("====================================\n");

#ifdef DEBUG
    printf("  Mode: DEBUG\n");
    printf("  Sample interval: %ds\n", DEFAULT_SAMPLE_INTERVAL);
#elif defined(TEST_MODE)
    printf("  Mode: TEST\n");
    printf("  Running self-test...\n");
#else
    printf("  Mode: RELEASE\n");
    printf("  Sample interval: %ds\n", DEFAULT_SAMPLE_INTERVAL);
#endif

#ifdef PLATFORM_ARM
    printf("  Platform: ARM\n");
#elif defined(PLATFORM_X86)
    printf("  Platform: x86\n");
#else
    printf("  Platform: unknown\n");
#endif

    printf("------------------------------------\n");

    sensor_init();
    platform_init();

    float reading = sensor_read();
    printf("\nSensor reading: %.2f\n", reading);

#ifdef ENABLE_LOGGING
    printf("[LOG] Reading logged successfully\n");
#endif

#ifdef DEBUG_UART
    printf("[UART] TX: sensor_reading=%.2f\n", reading);
#endif

    sensor_cleanup();
    platform_cleanup();

    printf("====================================\n");
    return 0;
}
