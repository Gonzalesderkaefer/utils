#include "../src/headers/path.h"
#include <assert.h>
#include <stdio.h>


int main(void) {
    Path *home = path_init("/home/user1");
    assert(home);
    path_append(home, ".config");
    puts(path_str(home));
}
