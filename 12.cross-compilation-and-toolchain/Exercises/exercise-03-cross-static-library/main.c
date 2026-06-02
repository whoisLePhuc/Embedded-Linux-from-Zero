#include <stdio.h>
#include "math_ops.h"

int main() {
    int x = 10, y = 3;

    printf("add(%d, %d) = %d\n", x, y, add(x, y));
    printf("sub(%d, %d) = %d\n", x, y, sub(x, y));
    printf("mul(%d, %d) = %d\n", x, y, mul(x, y));
    printf("div(%d, %d) = %d\n", x, y, div_op(x, y));

    return 0;
}
