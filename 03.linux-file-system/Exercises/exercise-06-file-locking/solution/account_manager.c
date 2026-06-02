#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <errno.h>

#define ACCOUNT_FILE "account.txt"

// Read current balance from file
int read_balance(int fd) {
    char buffer[32];
    lseek(fd, 0, SEEK_SET);
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    if (n <= 0) return 0;
    buffer[n] = '\0';
    return atoi(buffer);
}

// Write balance to file
void write_balance(int fd, int balance) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d\n", balance);
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);
    write(fd, buffer, strlen(buffer));
}

void display_menu() {
    printf("\n=== Account Manager ===\n");
    printf("1. Check Balance\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Exit\n");
    printf("Choice: ");
}

int check_balance() {
    int fd = open(ACCOUNT_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    // Use shared lock for reading (multiple readers OK)
    printf("Acquiring shared lock...\n");
    if (flock(fd, LOCK_SH) == -1) {
        perror("flock");
        close(fd);
        return -1;
    }
    
    int balance = read_balance(fd);
    printf("Current balance: $%d\n", balance);
    
    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}

int deposit_money() {
    int fd = open(ACCOUNT_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    int amount;
    printf("Enter amount to deposit: $");
    if (scanf("%d", &amount) != 1 || amount <= 0) {
        fprintf(stderr, "Invalid amount\n");
        close(fd);
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }
    
    // Use exclusive lock for writing
    printf("Acquiring exclusive lock...\n");
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock");
        close(fd);
        return -1;
    }
    printf("Lock acquired!\n");
    
    int balance = read_balance(fd);
    printf("Current balance: $%d\n", balance);
    printf("Depositing $%d...\n", amount);
    
    // Simulate some processing time
    sleep(2);
    
    balance += amount;
    write_balance(fd, balance);
    printf("New balance: $%d\n", balance);
    
    printf("Releasing lock...\n");
    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}

int withdraw_money() {
    int fd = open(ACCOUNT_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    int amount;
    printf("Enter amount to withdraw: $");
    if (scanf("%d", &amount) != 1 || amount <= 0) {
        fprintf(stderr, "Invalid amount\n");
        close(fd);
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }
    
    // Use exclusive lock for writing
    printf("Acquiring exclusive lock...\n");
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock");
        close(fd);
        return -1;
    }
    printf("Lock acquired!\n");
    
    int balance = read_balance(fd);
    printf("Current balance: $%d\n", balance);
    
    if (balance < amount) {
        printf("Insufficient funds!\n");
        flock(fd, LOCK_UN);
        close(fd);
        return -1;
    }
    
    printf("Withdrawing $%d...\n", amount);
    
    // Simulate some processing time
    sleep(2);
    
    balance -= amount;
    write_balance(fd, balance);
    printf("New balance: $%d\n", balance);
    
    printf("Releasing lock...\n");
    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}

int main() {
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
                check_balance();
                break;
            case 2:
                deposit_money();
                break;
            case 3:
                withdraw_money();
                break;
            case 4:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
