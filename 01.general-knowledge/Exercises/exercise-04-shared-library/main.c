#include <stdio.h>
#include "math_ops.h"

int main() {
    printf("=== Math Library Test ===\n\n");

    printf("add(10, 5)     = %d\n", add(10, 5));
    printf("subtract(10, 5)= %d\n", subtract(10, 5));
    printf("multiply(10, 5)= %d\n", multiply(10, 5));
    printf("divide(10, 5)  = %d\n", divide(10, 5));

    printf("\nfactorial(5)   = %d\n", factorial(5));
    printf("factorial(10)  = %d\n", factorial(10));

    printf("\ngcd(48, 18)   = %d\n", gcd(48, 18));
    printf("gcd(1071, 462) = %d\n", gcd(1071, 462));

    printf("\nAll tests passed!\n");
    return 0;
}
