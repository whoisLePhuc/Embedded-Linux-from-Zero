#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    double value;
    double result;
} thread_arg_t;

void *calc_sqrt(void *arg) {
    thread_arg_t *t = (thread_arg_t *)arg;
    t->result = sqrt(t->value);
    return NULL;
}

int main(int argc, char *argv[]) {
    double val = (argc > 1) ? atof(argv[1]) : 25.0;
    pthread_t tid;
    thread_arg_t arg = {.value = val, .result = 0.0};

    pthread_create(&tid, NULL, calc_sqrt, &arg);
    pthread_join(tid, NULL);

    printf("sqrt(%.0f) = %f\n", val, arg.result);
    printf("sin(%.0f) = %f\n", val, sin(val));
    printf("cos(%.0f) = %f\n", val, cos(val));

    return 0;
}
