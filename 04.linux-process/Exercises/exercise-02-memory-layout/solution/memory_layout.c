#include <stdio.h>
#include <stdlib.h>

// Global initialized variable -> Data segment
int global_init = 100;

// Global uninitialized variable -> BSS segment
int global_uninit;

// Static initialized variable -> Data segment
static int static_init = 200;

// Static uninitialized variable -> BSS segment
static int static_uninit;

void demo_function(void) {
    printf("  demo_function():   %p\n", (void*)demo_function);
}

int main() {
    // Local variable -> Stack segment
    int local_var = 300;
    int local_var2 = 400;
    
    // Dynamic allocation -> Heap segment
    int *heap_var1 = (int*)malloc(sizeof(int));
    int *heap_var2 = (int*)malloc(sizeof(int));
    
    if (heap_var1 == NULL || heap_var2 == NULL) {
        perror("malloc");
        return 1;
    }
    
    *heap_var1 = 500;
    *heap_var2 = 600;
    
    printf("==============================\n");
    printf("     Memory Layout Demo\n");
    printf("==============================\n\n");
    
    // Text segment (code)
    printf("TEXT Segment (Code):\n");
    printf("  main():            %p\n", (void*)main);
    demo_function();
    printf("\n");
    
    // Data segment (initialized)
    printf("DATA Segment (Initialized):\n");
    printf("  global_init:       %p (value: %d)\n", (void*)&global_init, global_init);
    printf("  static_init:       %p (value: %d)\n", (void*)&static_init, static_init);
    printf("\n");
    
    // BSS segment (uninitialized)
    printf("BSS Segment (Uninitialized):\n");
    printf("  global_uninit:     %p (value: %d)\n", (void*)&global_uninit, global_uninit);
    printf("  static_uninit:     %p (value: %d)\n", (void*)&static_uninit, static_uninit);
    printf("\n");
    
    // Heap segment (dynamic)
    printf("HEAP Segment (Dynamic):\n");
    printf("  heap_var1:         %p (value: %d)\n", (void*)heap_var1, *heap_var1);
    printf("  heap_var2:         %p (value: %d)\n", (void*)heap_var2, *heap_var2);
    printf("  (Heap grows upward: heap_var2 > heap_var1)\n");
    printf("\n");
    
    // Stack segment (local)
    printf("STACK Segment (Local):\n");
    printf("  local_var:         %p (value: %d)\n", (void*)&local_var, local_var);
    printf("  local_var2:        %p (value: %d)\n", (void*)&local_var2, local_var2);
    printf("  (Stack grows downward: local_var2 < local_var)\n");
    printf("\n");
    
    printf("==============================\n");
    printf("Address Order (High to Low):\n");
    printf("  STACK > HEAP > BSS > DATA > TEXT\n");
    printf("==============================\n");
    
    // Cleanup
    free(heap_var1);
    free(heap_var2);
    
    return 0;
}
