#include <stdio.h>
#include "platform.h"

#ifdef PLATFORM_ARM
    #define GPIO_BASE 0x3F200000
#elif defined(PLATFORM_X86)
    #define GPIO_BASE 0x0
#else
    #define GPIO_BASE 0xDEAD0000
#endif

int platform_init(void) {
    printf("[PLATFORM] Init: GPIO base at 0x%X\n", GPIO_BASE);

#ifdef ENABLE_LOGGING
    printf("[PLATFORM] Logging enabled\n");
#endif

#ifdef DEBUG_UART
    printf("[PLATFORM] UART debug on /dev/ttyAMA0\n");
#endif

    return 0;
}

void platform_cleanup(void) {
    printf("[PLATFORM] Cleanup: GPIO released\n");
}
