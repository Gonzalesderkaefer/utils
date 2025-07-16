#include "../src/htab_template/htab_temp.h"
#include <stdlib.h>
#include <stdio.h>

HtabDef(double, int);
HtabImpl(double, int);






int main(void) {
    HtabOf(double, int) *htab = htab_init_double_int(malloc, free);
    htab_insert_double_int(htab, 3, 3.3);
    printf("%d\n", *htab_lookup_double_int(htab, 3.3));
    htab_free_double_int(htab);
}

