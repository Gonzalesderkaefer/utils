#include "minlist.h"
#include <stdio.h>


#define getint(NAM) double doub_##NAM = 2;



int main(void) {
    char **array = NULL;

    list_init(array);
    list_push(array, "Hello");
    list_push(array, "World");
    list_push(array, "How's it goin'");

    for (int i = 0; i < list_len(array); ++i)
        printf("%s\n", array[i]);

    free(array);




}
