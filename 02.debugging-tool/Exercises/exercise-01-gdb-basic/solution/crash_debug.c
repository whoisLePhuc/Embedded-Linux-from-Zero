#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[16];
    float score;
} Student;

void print_students(Student *students[], int count) {
    for (int i = 0; i < count; i++) {
        if (students[i] != NULL) {
            printf("Student %d: %s - %.1f\n",
                   i + 1, students[i]->name, students[i]->score);
        }
    }
}

void add_student(Student *students[], int index, const char *name, float score) {
    Student *s = malloc(sizeof(Student));
    strcpy(s->name, name);
    s->score = score;
    students[index] = s;
}

int main() {
    Student *students[3];

    add_student(students, 0, "Nguyen Van A", 8.5);
    add_student(students, 1, "Tran Thi B", 7.2);

    Student *c = malloc(sizeof(Student));
    strcpy(c->name, "Le Van C");
    c->score = 9.0;
    students[2] = c;

    print_students(students, 3);

    for (int i = 0; i < 3; i++) {
        if (students[i] != NULL) {
            free(students[i]);
        }
    }

    return 0;
}
