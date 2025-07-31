// Header file
#include "htab.h"

// Libraries
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>


typedef struct _Pair {
    size_t keylen;
    char *key; // Does not have to be a c-style string (will always be allocated)
    void *value;
} Pair;



typedef struct _Htab {
    size_t length;
    size_t capacity;
    AllocFunc alloc;
    FreeFunc dealloc;
    Pair *storage;
} Htab;


Htab *htab_init(const AllocFunc alloc, const FreeFunc dealloc) {
    // Allocate struct
    Htab *new_table = alloc(sizeof(Htab));
    if (new_table == NULL) {
        return NULL;
    }

    // Allocate storage
    Pair *new_storage = alloc(sizeof(Pair) * INIT_SIZE);
    if (new_storage == NULL) {
        dealloc(new_table);
        return NULL;
    }
    // Zero out memory
    memset(new_storage, 0, sizeof(Pair) * INIT_SIZE);

    // Assign struct fields
    new_table->alloc = alloc == NULL ? malloc : alloc;
    new_table->dealloc = dealloc == NULL ? free : dealloc;
    new_table->capacity = INIT_SIZE;
    new_table->length = 0;
    new_table->storage = new_storage;


    return new_table;
}

// NOTE: See (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
static uint64_t fnv1a_hash(const char *key, const size_t length) {
    uint64_t hash = 14695981039346656037LU;
    for (int i = 0; i < length; ++i) {
        hash ^= key[i];
        hash *= 1099511628211;
    }
    return hash;
}



static void reinsert_after_grow(Htab *htab, void *value, void *key, size_t keylen) {
    // Sanity check
    if (value == NULL || key == NULL || keylen == 0) {
        return;
    }


    // Calculate hash and index
    const uint64_t hash = fnv1a_hash(key, keylen);
    const uint64_t index = hash % htab->capacity;

    // If index is free.
    if (htab->storage[index].value == NULL) {
        htab->storage[index].keylen = keylen;
        htab->storage[index].key = key;
        htab->storage[index].value = value;
    } else {
        // Find different index
        uint64_t other_index = (index + 1) % htab->capacity;
        while (htab->storage[other_index].value != NULL) {
            // increment other_index
            other_index = (other_index + 1) % htab->capacity;
        }
        htab->storage[other_index].keylen = keylen;
        htab->storage[other_index].key = key;
        htab->storage[other_index].value = value;
    }
}


static void grow(Htab *htab) {
    // save storage
    Pair *old_storage = htab->storage;
    size_t old_cap = htab->capacity;

    // Allocate new storage
    Pair *new_storage = htab->alloc(sizeof(Pair) * old_cap * 2);
    if (new_storage == NULL) {
        return;
    }
    // Overwrite with 0
    memset(new_storage, 0, old_cap * 2);

    // Assign new_storage to htab
    htab->storage = new_storage;
    htab->capacity = old_cap * 2;

    for (uint64_t i = 0; i < old_cap; i++) {
        const Pair cur_pair = old_storage[i];

        // Insert into new storage
        reinsert_after_grow(
            htab,
            cur_pair.value,
            cur_pair.key,
            cur_pair.keylen
        );
    }

    // Cleanup old storage
    htab->dealloc(old_storage);
}




void htab_insert(Htab *htab, void *value, void *key, size_t value_len, size_t keylen) {
    // Sanity check
    if (value == NULL || key == NULL || keylen == 0) {
        return;
    }

    // Calculate hash and index
    const uint64_t hash = fnv1a_hash(key, keylen);
    const uint64_t index = hash % htab->capacity;

    // If index is free.
    if (htab->storage[index].value == NULL) {
        // Allocate key
        char *new_key = htab->alloc(keylen);
        if (new_key == NULL) {
            return;
        }
        // Copy key into allocated buffer
        memcpy(new_key, key, keylen);

        // Allocate value
        char *new_value = htab->alloc(keylen);
        if (new_value == NULL) {
            return;
        }
        // Copy value into allocated buffer
        memcpy(new_value, value, value_len);

        htab->storage[index].keylen = keylen;
        htab->storage[index].key = new_key;
        htab->storage[index].value = new_value;
    } else {
        // Find different index
        uint64_t other_index = (index + 1) % htab->capacity;
        while (htab->storage[other_index].value != NULL) {
            // If we reached the original index, we have to grow
            if (other_index == index) {
                grow(htab);
                //other_index = index;
            }
            // increment other_index
            other_index = (other_index + 1) % htab->capacity;
        }

        // Allocate key
        char *new_key = htab->alloc(keylen);
        if (new_key == NULL) {
            return;
        }
        // Copy key into allocated buffer
        memcpy(new_key, key, keylen);

        // Allocate value
        char *new_value = htab->alloc(keylen);
        if (new_value == NULL) {
            return;
        }
        // Copy value into allocated buffer
        memcpy(new_value, value, value_len);

        htab->storage[other_index].keylen = keylen;
        htab->storage[other_index].key = new_key;
        htab->storage[other_index].value = new_value;
    }
}

static bool key_match_pair(const Pair pair, const char *key, const size_t keylen) {
    // Sanity check
    if (pair.key == NULL || pair.value == NULL || key == NULL || keylen == 0) {
        return false;
    }


    // If key length is different, there's no point in 
    // keeping going.
    if (pair.keylen != keylen) {
        return false;
    }
    // Compare the actual memory
    if (memcmp(key, pair.key, keylen) == 0) {
        return true;
    }

    // Keylen was same but memory was different
    return false;

}

void *htab_lookup(Htab *htab, char *key, size_t keylen) {
    // Sanity check
    if (htab == NULL || key == NULL || keylen == 0) {
        return NULL;
    }

    // Calculate hash and index
    const uint64_t hash = fnv1a_hash(key, keylen);
    const uint64_t index = hash % htab->capacity;

    uint64_t other_index = index;
    while (!key_match_pair(htab->storage[other_index], key, keylen)) {
        other_index = (other_index + 1) % htab->capacity;
        if (other_index == index) {
            return NULL;
        }
    }

    // Value was found otherwise we'd still be in the loop
    return htab->storage[other_index].value;
}

void htab_free(Htab *htab) {
    for (int i = 0; i < htab->length; ++i) {
        // Check if Slot is occupied
        if (htab->storage[i].value != NULL) {
            // Free keys and values
            htab->dealloc(htab->storage[i].key);
            htab->dealloc(htab->storage[i].value);
        }
    }

    // Free storage
    htab->dealloc(htab->storage);

    // Save pointer to deallocation function
    void (*dealloc)(void *) = htab->dealloc;

    // Deallocate struct itself
    dealloc(htab);
}


AllocFunc htab_alloc(Htab *htab) {
    return htab->alloc;
}

FreeFunc htab_dealloc(Htab *htab) {
    return htab->dealloc;
}
