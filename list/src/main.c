#include "list/list.h"
#include <stdio.h>

int main(void) {

    List *new_list = list_init(sizeof(int));

    for (int i = 0; i < 13; ++i) {
        list_push(new_list, &i);
    }

    size_t len = list_len(new_list);
    int *array = list_storage(new_list);

    for (int i = 0; i < len; ++i) {
        printf("%d\n", array[i]);
    }
    list_free(new_list);
}
