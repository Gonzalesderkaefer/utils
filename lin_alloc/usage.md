
```C
#include "lin_alloc.h"



lin_init(); // Initialize global allocator and pointer to current allocator


void *this_alloc(size_t size){
    return lalloc(cur_mem, size);
}

void this_free(void *ptr){}


int main(void) {
    lin_switch(main_mem); // Create new local linear allocator called main_mem
                          // and switch to it

    lin_free(main_mem); // Free local allocator and switch to global allocator
}
```
