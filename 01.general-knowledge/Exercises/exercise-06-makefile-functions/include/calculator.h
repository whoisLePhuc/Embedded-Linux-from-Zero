#ifndef CALCULATOR_H
#define CALCULATOR_H

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} operation_t;

int calculate(int a, int b, operation_t op);

#endif /* CALCULATOR_H */
