#include <stdio.h>
#include "calculator.h"
#include "logger.h"

int main() {
    double a = 10.0, b = 5.0;
    
    log_init();
    log_message(LOG_INFO, "Starting Advanced Calculator");
    
    printf("\n=== ADVANCED CALCULATOR ===\n");
    printf("a = %.2f, b = %.2f\n\n", a, b);
    
    printf("Addition: %.2f + %.2f = %.2f\n", a, b, calc_add(a, b));
    printf("Subtraction: %.2f - %.2f = %.2f\n", a, b, calc_subtract(a, b));
    printf("Multiplication: %.2f * %.2f = %.2f\n", a, b, calc_multiply(a, b));
    printf("Division: %.2f / %.2f = %.2f\n", a, b, calc_divide(a, b));
    printf("Power: %.2f ^ 2 = %.2f\n", a, calc_power(a, 2));
    
    log_message(LOG_INFO, "All calculations completed successfully");
    
    return 0;
}
