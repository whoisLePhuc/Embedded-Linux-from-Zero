#include "calculator.h"

int calculate(int a, int b, operation_t op) {
    switch (op) {
        case OP_ADD:
            return a + b;
        case OP_SUB:
            return a - b;
        case OP_MUL:
            return a * b;
        case OP_DIV:
            return (b != 0) ? a / b : 0;
        default:
            return 0;
    }
}
