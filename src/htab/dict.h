#ifndef DICT_H
#define DICT_H

#include "htab.h"
#include <sys/types.h>
#include <string.h>

#ifdef HTAB_H

#define dict_init(alloc, dealloc) \
    htab_init(alloc, dealloc)

#define dict_put(dict, key_string, value) \
    void *allocated = allocator(dict)(sizeof(value)); \
    if (allocated != NULL) { \
        memcpy(allocated, &value, sizeof(value)); \
        htab_insert(dict, value, key_string, strlen(key_string) + 1, true) \
    } 

#define dict_get(dict, key_string) \
    htab_lookup(dict, key_string, strlen(key_string) + 1)

#define dict_free(dict) \
    htab_free(dict);





#endif //HTAB_H
#endif // DICT_H
