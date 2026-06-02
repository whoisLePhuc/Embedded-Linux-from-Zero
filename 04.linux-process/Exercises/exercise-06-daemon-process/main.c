#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define LOG_FILE "/tmp/daemon_timestamp.log"

/* TODO: Declare volatile sig_atomic_t flags for running and reload */

/* TODO: Implement signal_handler for SIGTERM and SIGHUP */

void daemonize(void) {
    /* TODO:
     * 1. Fork and let parent exit
     * 2. Create new session with setsid()
     * 3. Fork again to prevent acquiring terminal
     * 4. Set umask to 0
     * 5. Change working directory to /
     * 6. Close all file descriptors
     * 7. Redirect stdin/stdout/stderr to /dev/null
     */
}

int main(void) {
    /* TODO: Register signal handlers */

    daemonize();

    /* TODO:
     * 1. Open log file for append
     * 2. Loop while running flag is set
     *    - Handle reload flag (re-open log file on SIGHUP)
     *    - Write timestamp to log every 5 seconds
     *    - Flush log file
     * 3. On SIGTERM: write "shutdown" message and close log
     */

    return 0;
}
