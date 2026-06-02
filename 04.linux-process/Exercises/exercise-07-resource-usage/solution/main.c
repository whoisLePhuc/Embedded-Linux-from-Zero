#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void cpu_workload(void) {
    int count = 0;
    for (int i = 0; i < 500000; i++) {
        count += is_prime(i);
    }
    printf("Found %d primes\n", count);
}

void memory_workload(void) {
    size_t page_size = sysconf(_SC_PAGESIZE);
    int num_pages = 5000;
    char **blocks = malloc(num_pages * sizeof(char*));
    if (!blocks) return;

    for (int i = 0; i < num_pages; i++) {
        blocks[i] = malloc(page_size);
        if (blocks[i]) {
            blocks[i][0] = 'A';
        }
    }

    for (int i = 0; i < num_pages; i++) {
        free(blocks[i]);
    }
    free(blocks);
}

void print_rusage(const char *label, const struct rusage *u) {
    printf("--- %s ---\n", label);
    printf("User CPU time:        %ld.%06lds\n",
           u->ru_utime.tv_sec, u->ru_utime.tv_usec);
    printf("System CPU time:      %ld.%06lds\n",
           u->ru_stime.tv_sec, u->ru_stime.tv_usec);
    printf("Voluntary csw:        %ld\n", u->ru_nvcsw);
    printf("Involuntary csw:      %ld\n", u->ru_nivcsw);
    printf("Minor faults:         %ld\n", u->ru_minflt);
    printf("Major faults:         %ld\n", u->ru_majflt);
    printf("Max RSS:              %ld KB\n", u->ru_maxrss);
    printf("\n");
}

int main(void) {
    struct rusage usage_before, usage_after;
    struct timespec ts_start, ts_end;

    printf("====================================\n");
    printf("    Process Resource Usage Demo\n");
    printf("====================================\n\n");

    getrusage(RUSAGE_SELF, &usage_before);
    print_rusage("Before Workload", &usage_before);

    printf("--- Running CPU workload ---\n");
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    cpu_workload();
    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    getrusage(RUSAGE_SELF, &usage_after);
    print_rusage("After Workload (RUSAGE_SELF)", &usage_after);

    double wall = (ts_end.tv_sec - ts_start.tv_sec) +
                  (ts_end.tv_nsec - ts_start.tv_nsec) / 1e9;
    double user_diff = (usage_after.ru_utime.tv_sec - usage_before.ru_utime.tv_sec) +
                       (usage_after.ru_utime.tv_usec - usage_before.ru_utime.tv_usec) / 1e6;
    double sys_diff  = (usage_after.ru_stime.tv_sec - usage_before.ru_stime.tv_sec) +
                       (usage_after.ru_stime.tv_usec - usage_before.ru_stime.tv_usec) / 1e6;

    printf("--- Timing Summary ---\n");
    printf("Wall-clock time:      %.3fs\n", wall);
    printf("User CPU time used:   %.3fs\n", user_diff);
    printf("System CPU time used: %.3fs\n", sys_diff);
    printf("CPU utilization:      %.1f%%\n\n", 100.0 * (user_diff + sys_diff) / wall);

    printf("--- Forking child for memory workload ---\n");
    fflush(stdout);  /* flush buffers before fork to avoid child duplicating output */
    pid_t pid = fork();
    if (pid == 0) {
        memory_workload();
        printf("Child (PID %d): memory workload done\n", getpid());
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
        struct rusage children_usage;
        getrusage(RUSAGE_CHILDREN, &children_usage);
        print_rusage("After Child (RUSAGE_CHILDREN)", &children_usage);
    } else {
        perror("fork");
    }

    return 0;
}
