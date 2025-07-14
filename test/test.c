#include <assert.h>
#include <stdio.h>

//#include "../src/minlist/minlist.h"
#include "../src/list/list.h"
#include "../src/htab/htab.h"
#include "../src/htab/dict.h"
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
    lin_switch(htab_test);
    Htab *dictionary = dict_init(this_alloc, this_free);






    lin_free(htab_test);
}


int main(void) {
    htab_test();
}

