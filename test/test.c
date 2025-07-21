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

    assert(&six == tree_lookup(new_tree, to_ptr(6), sizeof(int)));

    //new_tree->root = balance_node(new_tree->root);
    printf("%ld\n", get_balance(new_tree->root));

}
