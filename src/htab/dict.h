#ifndef DICT_H
#define DICT_H

#include <sys/types.h>
#include <string.h>


#define dict_init(alloc, dealloc) \
    htab_init(alloc, dealloc)

#define dict_put(dict, key_string, value) \
    htab_insert(dict, allocated, key_string, strlen(key_string) + 1, true); \

#define dict_get(dict, key_string) \
    htab_lookup(dict, key_string, strlen(key_string) + 1)

#define dict_free(dict) \
    htab_free(dict);





#endif // DICT_H
