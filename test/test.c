#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/tree/tree.c"

#if (__STDC_VERSION__ >= 202300L)
#define to_ptr(value) (typeof(value) [1]){value}
#else
#define to_ptr(type, value) (type [1]){value}
#endif



int main(void) {
    int zero = 0;
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    int six = 6;
    Tree *new_tree = tree_init(malloc, free, memcmp, sizeof(int));
    tree_insert(new_tree, &three);
    tree_insert(new_tree, &two);
    tree_insert(new_tree, &one);
    tree_insert(new_tree, &four);
    tree_insert(new_tree, &five);
    tree_insert(new_tree, &six);
    tree_insert(new_tree, &zero);

    // new_tree->root->right_node = balance_node(new_tree->root->right_node);
    // printf("%ld\n", node_height(new_tree->root->right_node->left_node));
    // printf("%ld\n", node_height(new_tree->root->right_node->right_node));

    assert(new_tree->root->right_node->left_node->value == &five);
    assert(new_tree->root->right_node->right_node->value);
}
