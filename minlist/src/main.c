#include "minlist.h"
#include <stdio.h>


#define getint(NAM) double doub_##NAM = 2;



int main(void) {
    int *array = NULL;

    list_init(array);

    for (int i = 0; i < 50; ++i)
        list_push(array, i);

    free(array);




}
