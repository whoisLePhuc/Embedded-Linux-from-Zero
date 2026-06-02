#include <stdio.h>
#include <string.h>
#include "string_utils.h"

int main() {
    char str1[100] = "Hello Embedded Linux";
    char str2[100];
    
    printf("=== STRING UTILITIES DEMO ===\n\n");
    
    // Test str_length
    printf("Original string: '%s'\n", str1);
    printf("Length: %d\n\n", str_length(str1));
    
    // Test str_reverse
    strcpy(str2, str1);
    str_reverse(str2);
    printf("Reversed: '%s'\n\n", str2);
    
    // Test str_upper
    strcpy(str2, str1);
    str_upper(str2);
    printf("Uppercase: '%s'\n\n", str2);
    
    return 0;
}
