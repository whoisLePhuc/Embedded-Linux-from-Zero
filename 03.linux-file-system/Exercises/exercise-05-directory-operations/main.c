#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

long total_size = 0;

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void list_directory(const char *path, int indent) {
    DIR *dir;
    struct dirent *entry;

    // TODO 1: Open the directory using opendir()
    // If it fails, perror() and return



    // TODO 2: Read entries in a loop using readdir()

        // TODO 3: Skip "." and ".." entries



        // TODO 4: Build full path for stat()
        // Use snprintf to combine path + "/" + entry->d_name



        // TODO 5: Use stat() to get file information



        // TODO 6: Print indent, then entry name and file type
        // Hint: use S_ISREG(), S_ISDIR() macros on sb.st_mode
        // If it's a regular file, also print its size



        // TODO 7: If it's a directory, recursively call list_directory()
        // Pass the full_path and indent + 1



    // TODO 8: Close the directory using closedir()

}

int main(int argc, char *argv[]) {
    const char *start_path;

    // Use command-line argument if provided, otherwise default to "."
    if (argc > 1) {
        start_path = argv[1];
    } else {
        start_path = ".";
    }

    printf("Directory tree: %s\n", start_path);
    printf("========================\n");

    // TODO 9: Start recursive directory listing
    // Call list_directory() with start_path and indent 0

    printf("========================\n");
    printf("Total size of regular files: %ld bytes\n", total_size);

    return 0;
}
