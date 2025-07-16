#include "../src/htab_template/htab_temp.h"
#include <stdlib.h>
#include <stdio.h>

HtabDef(double, int);
HtabImpl(double, int);

typedef char * charptr;
HtabDef(double, charptr);
HtabImpl(double, charptr);




int main(void) {
    HtabOf(double, charptr) *names = htab_init_double_charptr(malloc, free);
    htab_insert_double_charptr(names, "Hello", 1.1);
    htab_insert_double_charptr(names, "World!", 1.2);
    htab_insert_double_charptr(names, "How you doin!", 1.3);
    htab_free_double_charptr(names);
}

