#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *head = NULL;

void insert_node(int value) {
    Node *new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        return;
    }

    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void insert_sorted(int value) {
    Node *new_node = malloc(sizeof(Node));
    new_node->value = value;

    if (head == NULL || head->value >= value) {
        new_node->next = head;
        head = new_node;
        return;
    }

    Node *current = head;
    while (current->next != NULL && current->next->value < value) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

int search_node(int value) {
    Node *current = head;
    while (current != NULL) {
        if (current->value == value)
            return 1;
        current = current->next;
    }
    return 0;
}

void free_list() {
    Node *current = head;
    int count = 0;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
        count++;
    }
    printf("Freed %d nodes\n", count);
}

void print_list() {
    Node *current = head;
    printf("List: ");
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    insert_node(10);
    insert_node(20);
    insert_node(30);
    insert_sorted(42);
    insert_node(50);

    print_list();

    if (search_node(30))
        printf("Search 30: found\n");
    else
        printf("Search 30: not found\n");

    if (search_node(99))
        printf("Search 99: found\n");
    else
        printf("Search 99: not found\n");

    free_list();
    return 0;
}
