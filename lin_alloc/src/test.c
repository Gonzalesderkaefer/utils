#include "lin_alloc.h"
#include <stdio.h>



lin_init();


void *this_alloc(size_t size){
    return lalloc(cur_mem, size);
}

void this_free(void *ptr){}


int main(void) {
    lin_switch(main_mem);
    lin_free(main_mem);
}











