#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 256
#define MAX_ARGS 64

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    
    printf("Simple Shell - Type 'exit' to quit\n");
    printf("====================================\n\n");
    
    while (1) {
        // Display prompt
        printf("myshell> ");
        fflush(stdout);
        
        // Read command
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break;  // EOF (Ctrl+D)
        }
        
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }
        
        // Check for exit command
        if (strcmp(line, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        // Parse command into arguments
        int argc = 0;
        char *token = strtok(line, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;  // argv must end with NULL
        
        // Skip if no arguments
        if (argc == 0) {
            continue;
        }
        
        // Fork and exec
        pid_t pid = fork();
        
        if (pid < 0) {
            // Fork failed
            perror("fork failed");
            continue;
        } 
        else if (pid == 0) {
            // Child process: execute command
            execvp(args[0], args);
            
            // If execvp returns, it failed
            perror("exec failed");
            _exit(1);  // Use _exit() in child after failed exec
        } 
        else {
            // Parent process: wait for child
            int status;
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0) {
                    printf("[Exit code: %d]\n", exit_code);
                }
            } else if (WIFSIGNALED(status)) {
                printf("[Killed by signal: %d]\n", WTERMSIG(status));
            }
        }
    }
    
    return 0;
}
