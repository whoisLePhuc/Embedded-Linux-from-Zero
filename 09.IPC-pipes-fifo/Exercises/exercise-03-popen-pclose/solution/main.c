#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 256

int run_read_command(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("popen (read)");
        return -1;
    }

    char line[MAX_LINE];
    printf("--- Output of: %s ---\n", cmd);
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    printf("--- End of output ---\n");

    int status = pclose(fp);
    if (status == -1) {
        perror("pclose");
        return -1;
    }

    if (WIFEXITED(status)) {
        printf("Command exited with status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}

int run_write_command(const char *cmd) {
    FILE *fp = popen(cmd, "w");
    if (fp == NULL) {
        perror("popen (write)");
        return -1;
    }

    // Write sample data to the command's stdin
    fprintf(fp, "5\n");
    fprintf(fp, "3\n");
    fprintf(fp, "1\n");
    fprintf(fp, "4\n");
    fprintf(fp, "2\n");

    printf("Data written to: %s\n", cmd);

    int status = pclose(fp);
    if (status == -1) {
        perror("pclose");
        return -1;
    }

    if (WIFEXITED(status)) {
        printf("Command exited with status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}

int main() {
    char command[MAX_LINE];
    int choice;

    printf("=== popen/pclose Shell Pipe Demo ===\n");
    printf("1. Read output from shell command\n");
    printf("2. Write input to shell command\n");
    printf("Choice: ");
    scanf("%d", &choice);
    getchar(); // consume newline

    printf("Enter shell command: ");
    fgets(command, MAX_LINE, stdin);
    command[strcspn(command, "\n")] = 0;

    if (choice == 1) {
        run_read_command(command);
    } else if (choice == 2) {
        run_write_command(command);
    } else {
        printf("Invalid choice.\n");
        return 1;
    }

    return 0;
}
