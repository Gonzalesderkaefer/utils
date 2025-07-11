#include <stdint.h>
#include <stddef.h>






/*********************************** Macros ***********************************/









/******************************** Functions ***********************************/

static uint64_t fnv1a_hash(const char *key, const size_t length) {
    uint64_t hash = 14695981039346656037LU;
    for (int i = 0; i < length; ++i) {
        hash ^= key[i];
        hash *= 1099511628211;
    }
    return hash;
}
