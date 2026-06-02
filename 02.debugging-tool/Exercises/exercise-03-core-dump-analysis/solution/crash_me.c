#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *create_node(int value) {
    TreeNode *node = malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert(TreeNode **root, int value) {
    if (*root == NULL) {
        *root = create_node(value);
        return;
    }
    if (value < (*root)->value)
        insert(&(*root)->left, value);
    else
        insert(&(*root)->right, value);
}

int tree_height(TreeNode *root) {
    if (root == NULL)
        return 0;
    int left = tree_height(root->left);
    int right = tree_height(root->right);
    return (left > right ? left : right) + 1;
}

void print_inorder(TreeNode *root) {
    if (root == NULL) return;
    print_inorder(root->left);
    printf("%d ", root->value);
    print_inorder(root->right);
}

void free_tree(TreeNode *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main() {
    TreeNode *root = NULL;
    int values[] = {30, 10, 50, 20, 40};

    for (int i = 0; i < 5; i++) {
        insert(&root, values[i]);
    }

    printf("Tree height: %d\n", tree_height(root));

    printf("In-order: ");
    print_inorder(root);
    printf("\n");

    free_tree(root);

    return 0;
}
