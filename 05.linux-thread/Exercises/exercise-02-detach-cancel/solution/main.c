#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define WORK_TIME 3

typedef struct {
    int id;
    char *buffer;
} WorkerContext;

void cleanup(void *arg) {
    WorkerContext *ctx = (WorkerContext *)arg;
    printf("[Cleanup] Worker %d: freeing buffer at %p\n",
           ctx->id, (void *)ctx->buffer);
    free(ctx->buffer);
    free(ctx);
}

void *worker(void *arg) {
    int id = *(int *)arg;

    if (id == 1) {
        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
        printf("[Worker %d] Started with DEFERRED cancellation\n", id);
    } else {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
        printf("[Worker %d] Started with ASYNCHRONOUS cancellation\n", id);
    }

    pthread_detach(pthread_self());

    WorkerContext *ctx = malloc(sizeof(WorkerContext));
    ctx->id = id;
    ctx->buffer = malloc(64);
    snprintf(ctx->buffer, 64, "Worker-%d-data", id);

    pthread_cleanup_push(cleanup, ctx);

    while (1) {
        printf("[Worker %d] Processing task... (buf: %s)\n", id, ctx->buffer);
        sleep(1);
    }

    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t workers[2];
    int ids[2] = {1, 2};

    for (int i = 0; i < 2; i++) {
        if (pthread_create(&workers[i], NULL, worker, &ids[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    printf("Main: Workers running, waiting %d seconds...\n", WORK_TIME);
    sleep(WORK_TIME);

    printf("Main: Cancelling all workers...\n");
    for (int i = 0; i < 2; i++) {
        if (pthread_cancel(workers[i]) != 0) {
            perror("pthread_cancel");
        }
    }

    sleep(1);
    printf("Main: All workers cancelled\n");

    return 0;
}
