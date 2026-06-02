#include "calculator.h"
#include <math.h>

double calc_add(double a, double b) {
    return a + b;
}

double calc_subtract(double a, double b) {
    return a - b;
}

double calc_multiply(double a, double b) {
    return a * b;
}

double calc_divide(double a, double b) {
    if (b == 0.0) {
        return NAN;
    }
    return a / b;
}

double calc_power(double base, int exponent) {
    return pow(base, exponent);
}
