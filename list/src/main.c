#include "list/list.h"
#include <stdio.h>

int main(void) {
    int one = 1;
    int two = 2;
    int three = 3;

    List *new_list = list_init(sizeof(int));

    list_push(new_list, &one);
    list_push(new_list, &two);
    list_push(new_list, &three);

    size_t len = list_len(new_list);
    int *array = list_storage(new_list);

    for (int i = 0; i < len; ++i)
        printf("%d\n", array[i]);

    list_free(new_list);
}
