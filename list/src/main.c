#include "list/list.h"
#include <stdio.h>

int main(void) {

    List *new_list = list_init(sizeof(char *));
    char *hello1 = "hello1";
    char *hello2 = "hello2";
    char *hello3 = "hello3";

    list_push(new_list, &hello1);
    list_push(new_list, &hello2);
    list_push(new_list, &hello3);

    char **arr = list_storage(new_list);
    for (int i = 0; i < list_len(new_list); ++i) {
        putc(arr[i][0], stdout);
    }

    list_free(new_list);
}
