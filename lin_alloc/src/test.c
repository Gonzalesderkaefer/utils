#include "lin_alloc.h"




LinAlloc glob_mem = {0};

#define talloc(size) lalloc(&glob_mem, size)

int main(void) {
    talloc(33);
    talloc(33);
    talloc(33);
    talloc(33);
    talloc(33);
    talloc(33);

    lfree(&glob_mem);
}
