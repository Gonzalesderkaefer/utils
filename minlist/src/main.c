#include "minlist.h"
#include <stdio.h>


int main(void) {
    int *array = NULL;

    list_init(array);

    for (int i = 0; i < 50000; ++i)
        list_push(array, i);

    free(array);
}
