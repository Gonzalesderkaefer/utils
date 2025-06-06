

// Libraries
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <strings.h>


#define INIT_SIZE 16


typedef struct _HTPair {
    void *value;
    size_t keylen;
    char *key; // This does not have to be a string
} HTPair;


typedef struct _HashTable {
    HTPair *storage;
    size_t capacity;
    size_t size;
} HashTable;




HashTable *htab_init(void) {
    // Allocate HashTable struct
    HashTable *new_ht = (HashTable *)malloc(sizeof(HashTable));
    if (new_ht == NULL) { // Error checking
        return NULL;
    }


    // Allocate Array of pairs
    new_ht->storage = (HTPair *)calloc(16, sizeof(HTPair));
    if (new_ht->storage == NULL) { // Error checking
        free(new_ht); // We have to free this ...
        return NULL;
    }

    // Assign rest of the pairs
    new_ht->capacity = INIT_SIZE;
    new_ht->size = 0;


    return new_ht;
}

// NOTE: See (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
static uint64_t fnv1a_hash(char *key, size_t length) {
    uint64_t hash = 14695981039346656037LU;
    for (int i = 0; i < length; ++i) {
        hash ^= key[i];
        hash *= 1099511628211;
    }
    return hash;
}


void htab_put(HashTable *ht, char *key, size_t keylen, void *value) {
    // get index from key
    const uint64_t index = fnv1a_hash(key, keylen) % ht->capacity;

    // Check if there's nothing at index
    if (ht->storage[index].value == NULL) {
        ht->storage[index].key = (char *)malloc(keylen);
        if (ht->storage[index].key == NULL) { // Error checking
            return; // Just return
        }

        // Copy key into buffer
        ht->storage[index].key = memcpy(ht->storage[index].key, key, keylen);

        // Assign value
        ht->storage[index].value = value;

        // Assign keylength
        ht->storage[index].keylen = keylen;

        return;
    }

    // If there's something at the index find next index
    uint64_t other_index = index + 1;
    while(ht->storage[other_index++ % ht->capacity].value && other_index != index);
    if (other_index == index) {
        return; // We probably have to resize
    } else {
        ht->storage[other_index].key = (char *)malloc(keylen);
        if (ht->storage[other_index].key == NULL) { // Error checking
            return; // Just return
        }

        // Copy key into buffer
        ht->storage[other_index].key = memcpy(ht->storage[index].key, key, keylen);

        // Assign value
        ht->storage[other_index].value = value;

        // Assign keylength
        ht->storage[other_index].keylen = keylen;

    }
}


void htab_free(HashTable *ht) {
    // Cleanup pairs
    for (uint32_t i = 0; i < ht->size; ++i) {
        // free key
        free(ht->storage[i].key);
    }

    // free storage array
    free(ht->storage);

    // free struct itself 
    free(ht);
}
