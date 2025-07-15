#include "../src/lin_alloc/lin_alloc.h"
#include "../src/list/temp_list.h"
#include <stdio.h>

lin_init();

void *this_alloc(size_t size) {
    return lalloc(cur_mem, size);
}

void this_free(void *ptr) {}


void list_test(void) {
    ListOf(int) new_list = {.alloc = this_alloc, .dealloc = this_free};
    list_push(new_list, 0);
    list_push(new_list, 1);
    list_push(new_list, 2);
    list_push(new_list, 3);
    list_push(new_list, 4);
    list_push(new_list, 5);
    list_push(new_list, 7);
    list_push(new_list, 8);
    list_push(new_list, 9);
    list_push(new_list, 10);
    list_push(new_list, 11);
    list_push(new_list, 12);
    list_push(new_list, 13);
    for (int i = 0; i < new_list.length; ++i) {
        printf("%d\n", new_list.storage[i]);
    }
}


void htab_test(void) {
}


int main(void) {
    list_test();
    lfree(cur_mem);
}

