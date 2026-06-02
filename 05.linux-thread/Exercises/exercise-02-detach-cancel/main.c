#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define WORK_TIME 3

/*
 * TODO: Define cleanup handler function
 *   - Takes a void* argument (the allocated buffer)
 *   - Print: "[Cleanup] Worker <id>: freeing buffer at <address>"
 *   - Free the buffer
 */

/*
 * TODO: Define worker thread function
 * Steps:
 *   1. Cast arg to int* to get thread ID
 *   2. Set cancel type based on ID:
 *      - ID 1: PTHREAD_CANCEL_DEFERRED
 *      - ID 2: PTHREAD_CANCEL_ASYNCHRONOUS
 *   3. Print: "[Worker <id>] Started with <type> cancellation"
 *   4. Detach self: pthread_detach(pthread_self())
 *   5. Allocate a 64-byte buffer on heap (use malloc)
 *   6. Push cleanup handler with buffer as argument
 *   7. Infinite loop: print "[Worker <id>] Processing task..." and sleep(1)
 *   8. Pop cleanup with execute=0 (never reached, but required for pairing)
 * Return: NULL
 */

int main() {
    pthread_t workers[2];
    int ids[2] = {1, 2};

    /*
     * TODO: Create 2 worker threads
     *   - Worker with ID 1: will use DEFERRED cancellation
     *   - Worker with ID 2: will use ASYNCHRONOUS cancellation
     *   - Check return value and print error if creation fails
     */

    printf("Main: Workers running, waiting %d seconds...\n", WORK_TIME);
    sleep(WORK_TIME);

    printf("Main: Cancelling all workers...\n");
    /*
     * TODO: Cancel both workers using pthread_cancel()
     *   - Check return value for errors
     */

    sleep(1);
    printf("Main: All workers cancelled\n");

    return 0;
}
