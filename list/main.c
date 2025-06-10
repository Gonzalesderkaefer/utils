#include "minlist.c"
#include <stdio.h>


#define getint(NAM) double doub_##NAM = 2;



int main(void) {
    int *array = NULL;

    list_init(array);
    list_push(array, 3);
    list_push(array, 2);
    list_push(array, 1);

    for (int i = 0; i < list_len(array); ++i)
        printf("%d\n", array[i]);

    free(array);




}
