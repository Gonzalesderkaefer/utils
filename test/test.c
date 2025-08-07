#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "../src/list/reflist/reflist.h"
#include "../src/list/dynlist/dynlist.h"
#include "../src/htab/htab.h"
#include "../src/list/vector.h"
#include "../src/tree/tree.h"

#define to_ptr(type, value) (type [1]){value}

#define n_deref(type, ptr, default) \
    ptr == NULL ? default : *(type *)ptr
#define offs(st, m) \
    ((size_t)&(((st *)0)->m))

#define off(ptr, m) \
    ((size_t)&(ptr->m))



// Vector of integer
#include "../src/templates/vector_template/vector.h"


// in case the type is a pointer 
#define PRETTY_TYPE intptr
#define TYPE int *

// Helper macros to force expansion before pasting
#define PASTE_FIRST(a, b) a##b
#define function(a, b) PASTE_FIRST(a, b)

// Now use this to create your function name
void function(do_something_with_, PRETTY_TYPE)(void) {
}

void test_reflist(void) {
    RefList *list = reflist_init_def();

    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    int six = 6;
    int seven = 7;
    int eight = 8;
    int nine = 9;
    int ten = 10;
    int eleven = 11;

    reflist_push(list, &one); 
    reflist_push(list, &two); 
    reflist_push(list, &three);
    reflist_push(list, &four);
    reflist_push(list, &five);
    reflist_push(list, &six); 
    reflist_push(list, &seven);
    reflist_push(list, &eight);
    reflist_push(list, &nine);
    reflist_push(list, &ten); 
    reflist_push(list, &eleven);

    assert(reflist_at(list, 0) == &one);
    assert(reflist_at(list, 3) == &four);
    assert(reflist_at(list, 10) == &eleven);





    reflist_free(list);
}


void test_dynlist(void) {
    DynList *list = dynlist_init_def(sizeof(int));

    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    int six = 6;
    int seven = 7;
    int eight = 8;
    int nine = 9;
    int ten = 10;
    int eleven = 11;

    dynlist_push(list, &one); 
    dynlist_push(list, &two); 
    dynlist_push(list, &three);
    dynlist_push(list, &four);
    dynlist_push(list, &five);
    dynlist_push(list, &six); 
    dynlist_push(list, &seven);
    dynlist_push(list, &eight);
    dynlist_push(list, &nine);
    dynlist_push(list, &ten);
    dynlist_push(list, &eleven);


    printf("%d\n", *(int *)dynlist_at(list, 0));
    printf("%d\n", *(int *)dynlist_at(list, 10));





    dynlist_free(list);
}

void test_htab(void) {
    Htab *table = htab_init(malloc, free);

    htab_put(table, int, double, 5, 6.9);
    htab_put(table, int, double, 4, 6.5);
    htab_put(table, int, double, 3, 4.4);
    htab_put(table, int, double, 2, 2.4);
    htab_put(table, int, double, 1, 1.1);

    int *fiveptr = htab_get(table, double, 6.9);
    assert(fiveptr != NULL);
    int *fourptr = htab_get(table, double, 6.5);
    assert(fourptr != NULL);
    int *threeptr = htab_get(table, double, 4.4);
    assert(threeptr != NULL);
    int *oneptr = htab_get(table, double, 2.4);
    assert(oneptr != NULL);
    int *twoptr = htab_get(table, double, 1.1);
    assert(twoptr != NULL);


    htab_free(table);
}


void  test_dict(void) {
    Dict *dict = dict_init(malloc, free);

    dict_put(dict, int, 3, "three");
    printf("%d\n", n_deref(int, dict_get(dict, "three"), 0));
    dict_free(dict);
}


void macro_htab(void) {
    struct { long double key; int value; size_t size; char ch; } *hash = NULL;
    printf("%lu\n", ((size_t)&(hash->ch)));
    printf("%lu\n", sizeof(hash->key));
    printf("%lu\n", sizeof(hash->value));
}


void macro_list(void) {
    char *word = NULL;
    vec_push(word, 'H');
    vec_push(word, 'e');
    vec_push(word, 'l');
    vec_push(word, 'l');
    vec_push(word, 'o');
    vec_push(word, ' ');
    vec_push(word, 'w');
    vec_push(word, 'o');
    vec_push(word, 'r');
    vec_push(word, 'l');
    vec_push(word, 'd');
    vec_push(word, '\0');


    puts(word);
    vec_free(word);
}




void test_tree(void) {
}


void test_tree_macro(void) {
    int *int_tree = NULL;
    tree_put(int_tree, 44);
    const int *hell;
    tree_get(hell, int_tree, 44);

    printf("%d\n", *hell);
    tree_del(int_tree);
}

void test_vector_macro(void) {
}

int main(void) {
    test_tree_macro();
}
