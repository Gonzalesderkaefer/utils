#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/list/reflist/reflist.h"
#include "../src/list/dynlist/dynlist.h"

#if (__STDC_VERSION__ >= 202300L)
#define to_ptr(value) (typeof(value) [1]){value}
#else
#define to_ptr(type, value) (type [1]){value}
#endif



void test_reflist() {
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


void test_dynlist() {
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

    dynlist_free(list);
}

int main(void) {
    test_dynlist();
}
