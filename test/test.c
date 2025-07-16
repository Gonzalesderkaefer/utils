#include "../src/htab/htab_real_impl.c"
#include <stdlib.h>
#include <stdio.h>

HtabDef(double, int);
HtabImpl(double, int);



static bool key_match_pair_double_int(const PairOf(double, int) pair, const char *key, const size_t keylen) {
    // Sanity check
    if (key == NULL || keylen == 0) {
        return false;
    }


    // If key length is different, there's no point in 
    // keeping going.
    if (sizeof(pair.k) != keylen) {
        return false;
    }
    // Compare the actual memory
    if (memcmp(key, &pair.k, keylen) == 0) {
        return true;
    }

    // Keylen was same but memory was different
    return false;
}



int htab_lookup_double_int(HtabOf(double, int) *htab, double key) {
    // Sanity check
    if (htab == NULL) {
        return 0;
    }

    // Calculate hash and index
    const uint64_t hash = fnv1a_hash_double_int((char *)&key, sizeof(key));
    const uint64_t index = hash % htab->capacity;

    uint64_t other_index = index;
    while (!key_match_pair_double_int(htab->storage[other_index], (char *)&key, sizeof(key))) {
        other_index = (other_index + 1) % htab->capacity;
        if (other_index == index) {
            return 0;
        }
    }

    // Value was found otherwise we'd still be in the loop
    return htab->storage[other_index].v;
}

int main(void) {
    HtabOf(double, int) *htab = htab_init_double_int(malloc, free);
    htab_insert_double_int(htab, 3, 3.3);
    printf("%d\n", htab_lookup_double_int(htab, 3.3));
    htab_free_double_int(htab);
}

