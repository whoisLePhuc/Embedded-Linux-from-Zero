#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// Struct to pass data to thread
typedef struct {
    char name[50];
    int birth_year;
} Person;

void *calculate_age(void *arg) {
    // 1. Cast arg to Person*
    Person *p = (Person *)arg;
    
    printf("Thread: Hi %s, calculating age...\n", p->name);
    
    // 2. Allocate int memory for age result
    int *age_result = (int *)malloc(sizeof(int));
    if (age_result == NULL) {
        perror("Failed to allocate memory");
        pthread_exit(NULL);
    }
    
    // 3. Calculate age (2025 - birth_year)
    *age_result = 2025 - p->birth_year;
    
    // 4. Return result pointer
    return (void *)age_result;
}

int main() {
    pthread_t thread1, thread2;
    Person p1, p2;
    
    // Prepare data
    strcpy(p1.name, "Alice");
    p1.birth_year = 1995;
    
    strcpy(p2.name, "Bob");
    p2.birth_year = 2000;

    // Create threads
    if (pthread_create(&thread1, NULL, calculate_age, &p1) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }
    
    if (pthread_create(&thread2, NULL, calculate_age, &p2) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    int *age1, *age2;
    
    // Join threads and retrieve results
    if (pthread_join(thread1, (void **)&age1) != 0) {
        perror("Failed to join thread 1");
        return 1;
    }
    
    if (pthread_join(thread2, (void **)&age2) != 0) {
        perror("Failed to join thread 2");
        return 1;
    }
    
    if (age1 != NULL) {
        printf("Main: %s is %d years old\n", p1.name, *age1);
        free(age1); // Free allocated memory
    }
    
    if (age2 != NULL) {
        printf("Main: %s is %d years old\n", p2.name, *age2);
        free(age2); // Free allocated memory
    }

    return 0;
}
