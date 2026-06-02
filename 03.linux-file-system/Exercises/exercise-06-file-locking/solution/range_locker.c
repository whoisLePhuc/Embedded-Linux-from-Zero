#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#define RECORD_SIZE 100
#define DATA_FILE "records.dat"
#define NUM_RECORDS 10
#define LOCK_TIMEOUT 10  // seconds

// Global flag for signal handling
volatile sig_atomic_t interrupted = 0;

// Signal handler for graceful shutdown
void signal_handler(int signum) {
    (void)signum;  // Unused parameter
    interrupted = 1;
    printf("\n[Signal received - cleaning up...]\n");
}

// Setup signal handlers
void setup_signals(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGTERM, &sa, NULL);  // kill
}

// Initialize data file with empty records
int init_file(void) {
    int fd = open(DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    char record[RECORD_SIZE];
    for (int i = 0; i < NUM_RECORDS; i++) {
        memset(record, 0, RECORD_SIZE);
        snprintf(record, RECORD_SIZE, "Record %02d: Empty\n", i);
        
        ssize_t written = write(fd, record, RECORD_SIZE);
        if (written != RECORD_SIZE) {
            perror("write");
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    printf("✓ Initialized %d records in %s\n", NUM_RECORDS, DATA_FILE);
    return 0;
}

// Try to acquire lock with timeout
int try_lock_with_timeout(int fd, struct flock *lock, int timeout_sec) {
    time_t start_time = time(NULL);
    
    while (1) {
        // Check for interruption
        if (interrupted) {
            fprintf(stderr, "Lock acquisition interrupted\n");
            return -1;
        }
        
        // Try non-blocking lock
        if (fcntl(fd, F_SETLK, lock) == 0) {
            return 0;  // Lock acquired
        }
        
        if (errno != EACCES && errno != EAGAIN) {
            perror("fcntl lock");
            return -1;
        }
        
        // Check timeout
        time_t elapsed = time(NULL) - start_time;
        if (elapsed >= timeout_sec) {
            fprintf(stderr, "Lock timeout after %d seconds\n", timeout_sec);
            return -1;
        }
        
        // Wait a bit before retrying
        usleep(100000);  // 100ms
        
        if (elapsed % 2 == 0 && elapsed > 0) {
            static time_t last_msg = 0;
            if (elapsed != last_msg) {
                printf("Still waiting for lock... (%ld/%d seconds)\n", 
                       elapsed, timeout_sec);
                last_msg = elapsed;
            }
        }
    }
}

// Get information about who holds the lock
void check_lock_holder(int fd, off_t offset, off_t len) {
    struct flock check_lock;
    memset(&check_lock, 0, sizeof(check_lock));
    check_lock.l_type = F_WRLCK;
    check_lock.l_whence = SEEK_SET;
    check_lock.l_start = offset;
    check_lock.l_len = len;
    
    if (fcntl(fd, F_GETLK, &check_lock) == -1) {
        perror("fcntl F_GETLK");
        return;
    }
    
    if (check_lock.l_type != F_UNLCK) {
        printf("⚠ Record is locked by PID %d\n", check_lock.l_pid);
    }
}

// Lock a specific record and update it
int lock_and_update(int record_num, const char *data) {
    int fd;
    struct flock lock;
    char record[RECORD_SIZE];
    int result = 0;
    
    fd = open(DATA_FILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    // Setup lock for specific record
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;              // Write lock (exclusive)
    lock.l_whence = SEEK_SET;           // From beginning of file
    lock.l_start = record_num * RECORD_SIZE;
    lock.l_len = RECORD_SIZE;
    
    printf("Attempting to lock record %d...\n", record_num);
    
    // Check who might be holding the lock
    check_lock_holder(fd, lock.l_start, lock.l_len);
    
    // Try to acquire lock with timeout
    if (try_lock_with_timeout(fd, &lock, LOCK_TIMEOUT) == -1) {
        close(fd);
        return -1;
    }
    
    printf("✓ Lock acquired for record %d\n", record_num);
    
    // Critical section - read current data
    off_t offset = lseek(fd, record_num * RECORD_SIZE, SEEK_SET);
    if (offset == -1) {
        perror("lseek");
        result = -1;
        goto cleanup;
    }
    
    ssize_t bytes_read = read(fd, record, RECORD_SIZE);
    if (bytes_read == -1) {
        perror("read");
        result = -1;
        goto cleanup;
    }
    
    printf("Current data: %s", record);
    
    // Update record
    memset(record, 0, RECORD_SIZE);
    snprintf(record, RECORD_SIZE, "Record %02d: %s (PID:%d)\n", 
             record_num, data, getpid());
    
    // Simulate some processing
    printf("Updating record %d... (sleeping 3 seconds)\n", record_num);
    for (int i = 0; i < 3 && !interrupted; i++) {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
    
    if (interrupted) {
        printf("Update interrupted - rolling back\n");
        result = -1;
        goto cleanup;
    }
    
    // Write updated data
    offset = lseek(fd, record_num * RECORD_SIZE, SEEK_SET);
    if (offset == -1) {
        perror("lseek");
        result = -1;
        goto cleanup;
    }
    
    ssize_t bytes_written = write(fd, record, RECORD_SIZE);
    if (bytes_written != RECORD_SIZE) {
        perror("write");
        result = -1;
        goto cleanup;
    }
    
    // Ensure data is written to disk
    if (fsync(fd) == -1) {
        perror("fsync");
        result = -1;
        goto cleanup;
    }
    
    printf("✓ Record %d updated successfully\n", record_num);
    
cleanup:
    // Release lock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl unlock");
    } else {
        printf("✓ Lock released for record %d\n", record_num);
    }
    
    close(fd);
    return result;
}

// Display all records with read locks
int show_records(void) {
    int fd = open(DATA_FILE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    char record[RECORD_SIZE];
    struct flock lock;
    
    printf("\n=== All Records ===\n");
    
    for (int i = 0; i < NUM_RECORDS; i++) {
        // Setup read lock for each record
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_RDLCK;      // Shared read lock
        lock.l_whence = SEEK_SET;
        lock.l_start = i * RECORD_SIZE;
        lock.l_len = RECORD_SIZE;
        
        // Try to acquire read lock
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            if (errno == EACCES || errno == EAGAIN) {
                printf("Record %02d: [LOCKED - being modified]\n", i);
                continue;
            } else {
                perror("fcntl read lock");
                close(fd);
                return -1;
            }
        }
        
        // Read the record
        ssize_t bytes_read = read(fd, record, RECORD_SIZE);
        if (bytes_read == -1) {
            perror("read");
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            return -1;
        }
        
        if (bytes_read == RECORD_SIZE) {
            printf("%s", record);
        } else {
            printf("Record %02d: [Incomplete read]\n", i);
        }
        
        // Release read lock
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
    }
    
    printf("===================\n");
    close(fd);
    return 0;
}

// Test lock without modifying
int test_lock(int record_num) {
    int fd = open(DATA_FILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * RECORD_SIZE;
    lock.l_len = RECORD_SIZE;
    
    printf("Testing lock on record %d...\n", record_num);
    
    // Check current lock status
    check_lock_holder(fd, lock.l_start, lock.l_len);
    
    // Try non-blocking lock
    if (fcntl(fd, F_SETLK, &lock) == 0) {
        printf("✓ Record %d is available (unlocked)\n", record_num);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
    } else {
        if (errno == EACCES || errno == EAGAIN) {
            printf("✗ Record %d is currently locked\n", record_num);
        } else {
            perror("fcntl test lock");
        }
    }
    
    close(fd);
    return 0;
}

void display_menu() {
    printf("\n=== Range Locker ===\n");
    printf("1. Show All Records\n");
    printf("2. Lock and Update Record\n");
    printf("3. Test Lock Status\n");
    printf("4. Initialize File\n");
    printf("5. Exit\n");
    printf("Choice: ");
}

int main() {
    setup_signals();
    
    int choice;
    
    while (1) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        switch (choice) {
            case 1:
                show_records();
                break;
                
            case 2: {
                int record_num;
                char data[50];
                
                printf("Enter record number (0-%d): ", NUM_RECORDS - 1);
                if (scanf("%d", &record_num) != 1 || 
                    record_num < 0 || record_num >= NUM_RECORDS) {
                    fprintf(stderr, "Invalid record number\n");
                    // Clear input buffer
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                
                printf("Enter data to write: ");
                // Clear newline from previous scanf
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                
                if (fgets(data, sizeof(data), stdin) == NULL) {
                    fprintf(stderr, "Failed to read data\n");
                    break;
                }
                
                // Remove trailing newline
                size_t len = strlen(data);
                if (len > 0 && data[len-1] == '\n') {
                    data[len-1] = '\0';
                }
                
                lock_and_update(record_num, data);
                break;
            }
                
            case 3: {
                int record_num;
                printf("Enter record number (0-%d): ", NUM_RECORDS - 1);
                if (scanf("%d", &record_num) != 1 || 
                    record_num < 0 || record_num >= NUM_RECORDS) {
                    fprintf(stderr, "Invalid record number\n");
                    // Clear input buffer
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                test_lock(record_num);
                break;
            }
                
            case 4:
                init_file();
                break;
                
            case 5:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}