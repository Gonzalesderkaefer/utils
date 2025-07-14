#include <assert.h>
#include <stdio.h>

//#include "../src/minlist/minlist.h"
#include "../src/list/list.h"
#include "../src/htab/htab.h"
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
    int four = 4;
    int five = 5;
    int six = 6;

    htab_insert(htab, &one, "one", strlen("one") + 1, false);
    htab_insert(htab, &two, "two", strlen("two") + 1, false);
    htab_insert(htab, &three, "three", strlen("three") + 1, false);
    htab_insert(htab, &four, "four", strlen("four") + 1, false);
    htab_insert(htab, &five, "five", strlen("five") + 1, false);
    htab_insert(htab, &six, "six", strlen("six") + 1, false);

    assert(htab_lookup(htab, "one", strlen("one") + 1) == &one);
    assert(htab_lookup(htab, "two", strlen("two") + 1) == &two);
    assert(htab_lookup(htab, "three", strlen("three") + 1) == &three);
    assert(htab_lookup(htab, "four", strlen("four") + 1) == &four);
    assert(htab_lookup(htab, "five", strlen("five") + 1) == &five);
    assert(htab_lookup(htab, "six", strlen("six") + 1) == &six);

    lin_free(htab_test);
}


int main(void) {
    htab_test();
}

