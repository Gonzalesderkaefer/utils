// Header file


// Libraries
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>


typedef struct _Pair {
    size_t keylen;
    char *key; // Does not have to be a c-style string (will always be allocated)
    bool allocated; // If value is allocated
    void *value;
} Pair;



typedef struct _Htab {
    size_t length;
    size_t capacity;
    void *(*alloc)(size_t);
    void (*dealloc)(void *);
    Pair *storage;
} Htab;


Htab *htab_init(void *(*alloc)(size_t), void (*dealloc)(void *)) {
    // Allocators have to be specified
    if (alloc == NULL || dealloc == NULL) {
        return NULL;
    }

    // Allocate struct
    Htab *new_table = alloc(sizeof(Htab));
    if (new_table == NULL) {
        return NULL;
    }

    // Allocate storage
    Pair *new_storage = alloc(sizeof(Pair) * 16);
    if (new_storage == NULL) {
        dealloc(new_table);
        return NULL;
    }
    // Zero out memory
    memset(new_storage, 0, sizeof(Pair) * 16);

    // Assign struct fields
    new_table->alloc = alloc;
    new_table->dealloc = dealloc;
    new_table->capacity = 16;
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

// TODO: Implement this!!
void grow(Htab *htab);




void htab_insert(Htab *htab, void *value, void *key, size_t keylen, bool allocated) {
    // Sanity check
    if (value == NULL || key == NULL || keylen == 0) {
        return;
    }

    // Calculate hash and index
    const uint64_t hash = fnv1a_hash(key, keylen);
    const uint64_t index = hash % htab->capacity;

    // If index is free.
    if (htab->storage[index].value != NULL) {
        // Allocate key
        char *new_key = htab->alloc(keylen);
        if (new_key == NULL) {
            return;
        }
        htab->storage[index].keylen = keylen;
        htab->storage[index].key = new_key;
        htab->storage[index].value = value;
        htab->storage[index].allocated = allocated;
    } else {
        // Find different index
        uint64_t other_index = index + 1;
        while (htab->storage[other_index].value != NULL) {
            // If we reached the original index, we have
            // to grow
            if (other_index == index) {
                grow(htab);
                return;
            }
            // increment other_index
            other_index = (other_index + 1) % htab->capacity;
        }
        // Allocate key
        char *new_key = htab->alloc(keylen);
        if (new_key == NULL) {
            return;
        }
        htab->storage[other_index].keylen = keylen;
        htab->storage[other_index].key = new_key;
        htab->storage[other_index].value = value;
        htab->storage[other_index].allocated = allocated;
    }
}














