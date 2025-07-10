#include <stdio.h>

//#include "../src/minlist/minlist.h"
#include "../src/minlist/list.h"


void list_test() {
    char **names = NULL;
    list_init(names);
    list_push(names, "John");
    list_push(names, "David");
    list_action(names, puts);
    list_free(names);
}


int main(void) {
    list_test();
}

