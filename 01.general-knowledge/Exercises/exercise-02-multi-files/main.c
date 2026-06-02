#include <stdio.h>
#include "math_utils.h"

int main() {
    int a = 10, b = 5;
    
    printf("=== CHƯƠNG TRÌNH TÍNH TOÁN ===\n");
    printf("a = %d, b = %d\n\n", a, b);
    
    printf("a + b = %d\n", add(a, b));
    printf("a - b = %d\n", subtract(a, b));
    printf("a * b = %d\n", multiply(a, b));
    printf("a / b = %.2f\n", divide(a, b));
    
    return 0;
}
