#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>

/* TODO: Implement is_prime() to check if a number is prime */

/* TODO: Implement cpu_workload() - count primes up to 500000 */

/* TODO: Implement print_rusage() - display rusage fields */

int main(void) {
    /* TODO:
     * 1. Print banner
     * 2. Get and print initial RUSAGE_SELF
     * 3. Run CPU workload, measure wall-clock time with clock_gettime()
     * 4. Get and print RUSAGE_SELF after workload (show difference)
     * 5. Fork a child that runs memory workload
     * 6. Parent waits for child, then print RUSAGE_CHILDREN
     */

    return 0;
}
