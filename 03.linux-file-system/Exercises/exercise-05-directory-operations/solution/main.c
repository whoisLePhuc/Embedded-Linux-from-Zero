#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static long total_size = 0;

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

static void print_file_type(mode_t mode) {
    if (S_ISREG(mode))  printf("regular file");
    else if (S_ISDIR(mode))  printf("directory");
    else if (S_ISLNK(mode))  printf("symlink");
    else if (S_ISCHR(mode))  printf("char device");
    else if (S_ISBLK(mode))  printf("block device");
    else if (S_ISFIFO(mode)) printf("fifo/pipe");
    else                     printf("unknown");
}

static void print_file_type(mode_t mode);

static void list_directory(const char *path, int indent) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        print_indent(indent);
        perror(path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[4096];
        int n = snprintf(full_path, sizeof(full_path),
                         "%s/%s", path, entry->d_name);
        if (n >= (int)sizeof(full_path)) {
            print_indent(indent);
            printf("(path too long: %s/%s)\n", path, entry->d_name);
            continue;
        }

        struct stat sb;
        if (stat(full_path, &sb) == -1) {
            print_indent(indent);
            perror(entry->d_name);
            continue;
        }

        print_indent(indent);
        printf("|-- %s  [", entry->d_name);
        print_file_type(sb.st_mode);
        printf(", %ld bytes, 0%o]\n",
               (long)sb.st_size, (unsigned)(sb.st_mode & 0777));

        if (S_ISREG(sb.st_mode)) {
            total_size += sb.st_size;
        }

        if (S_ISDIR(sb.st_mode)) {
            list_directory(full_path, indent + 1);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *start_path;

    if (argc > 1) {
        start_path = argv[1];
    } else {
        start_path = ".";
    }

    printf("Directory tree: %s\n", start_path);
    printf("========================\n");

    list_directory(start_path, 0);

    printf("========================\n");
    printf("Total size of regular files: %ld bytes\n", total_size);

    return 0;
}
