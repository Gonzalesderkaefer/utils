#include <stdio.h>

//#include "../src/minlist/minlist.h"
#include "../src/list/list.h"
#include "../src/hash_table/htab.h"
#include "../src/lin_alloc/lin_alloc.h"

lin_init();

void *this_alloc(size_t size) {
    return lalloc(cur_mem, size);
}

void this_free(void *ptr) {}


void list_test(void) {
    char **names = NULL;
    list_init(names);
    list_push(names, "John");
    list_push(names, "David");
    list_action(names, puts);
    list_free(names);
}


void htab_test(void) {
    lin_switch(htab_test)
    Htab *htab = htab_init(this_alloc, this_free);
    int one = 1;
    int two = 2;
    int three = 3;

    htab_insert(htab, &one, "one", 4, false);
    htab_insert(htab, &two, "two", 4, false);
    htab_insert(htab, &three, "three", 6, false);

    int *one_ptr = htab_lookup(htab, "one", 4);
    printf("%p\n", one_ptr);
    
    int *two_ptr = htab_lookup(htab, "two", 4);
    printf("%p\n", two_ptr);
    
    int *three_ptr = htab_lookup(htab, "three", 6);
    printf("%p\n", three_ptr);

    lin_free(htab_test);
}


int main(void) {
    htab_test();
}

