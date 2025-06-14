// Header file
#include "hash_table.h"

// Libraries
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/types.h>




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


void htab_put(HashTable *ht, const char *key, const size_t keylen, void *value);


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
    new_ht->capacity = 16;
    new_ht->size = 0;


    return new_ht;
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



static void grow(HashTable *ht, size_t new_size) {
    if (new_size <= ht->size) // Cannot be shrunken
        return;

    // Allocate new storage
    HTPair *new_storage = (HTPair *)calloc(new_size, sizeof(HTPair));
    if (new_storage == NULL) { // Error checking
        return;
    }

    // Save reference to to old storage and old size
    HTPair *old_storage = ht->storage;
    size_t old_size = ht->size;

    // Assign new storage and new size to ht
    ht->storage = new_storage;
    ht->size = new_size;


    // Insert old entries into new storage
    for (size_t i = 0; i < old_size; ++i) {
        if (old_storage[i].value != NULL) {
            htab_put(ht, old_storage[i].key, old_storage[i].keylen, old_storage[i].value);
        }
    }


    // Cleanup old pairs
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->storage[i].key) {
            free(ht->storage[i].key);
        }
    }
}

void htab_put(HashTable *ht, const char *key, const size_t keylen, void *value) {
    // Sanity check
    if (ht == NULL || key == NULL || value == NULL) {
        return;
    }


    // get index from key
    const uint64_t index = fnv1a_hash(key, keylen) % ht->capacity;
    printf("Calculated index is at: %lu for %s\n", index, key);

    // Check if there's nothing at index
    if (ht->storage[index].value == NULL) {
        printf("Insert index is at: %lu for %s\n", index, key);
        ht->storage[index].key = malloc(keylen);
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
    while((other_index % ht->capacity) != index) {
        if (ht->storage[other_index % ht->capacity].keylen == 0) {
            printf("Found empty slot at: %lu\n", other_index);
            break;
        } else {
            printf("Slot was occupied keep going: %lu\n", other_index);
            other_index += 1;
        }
    }

    printf("Insert index is at: %lu for %s\n", other_index, key);
    if (other_index == index) { // We have cycled through the list and have to grow
        puts("Had to grow");
        //grow(ht, ht->capacity * 2);
    } else {
        ht->storage[other_index].key = malloc(keylen);
        if (ht->storage[other_index].key == NULL) { // Error checking
            return; // Just return
        }

        // Copy key into buffer
        memcpy(ht->storage[other_index].key, key, keylen);

        // Assign value
        ht->storage[other_index].value = value;

        // Assign keylength
        ht->storage[other_index].keylen = keylen;

    }
}

void *htab_see(const HashTable *ht, const char *key, const size_t keylen) {
    // get index from key
    const uint64_t index = fnv1a_hash(key, keylen) % ht->capacity;
    printf("Calculated index is at: %lu for %s\n", index, key);

    // Check if value could even exist
    if (ht->storage[index].value == NULL)
        return NULL;

    // Check if correct entry is found
    if (ht->storage[index].keylen == keylen && // Check if keylen is same
            !memcmp(ht->storage[index].key, key, keylen)) { // Check if key is equal
        printf("Lookup index is at: %lu for %s\n", index, key);
        return ht->storage[index].value; // Found correct value so return
    }
    // Search for other index
    uint64_t other_index = index + 1;
    while((other_index % ht->capacity) != index) {
        printf("Lookup index is at: %lu \n", other_index);
        if (ht->storage[other_index].keylen == keylen && // Check if keylen is same
                memcmp(ht->storage[other_index].key, key, keylen) == 0) { // Check if key is equal
            printf("Found at %lu\n", other_index);
            return ht->storage[other_index].value; // Found correct value so return
        } else {
            printf("Key not found at %lu\n", other_index);
            other_index++;
        }
    }
    printf("Key not found.\n");
    return NULL;
}



void htab_free(HashTable *ht) {
    // Cleanup pairs
    for (uint64_t i = 0; i < ht->capacity; ++i) {
        if (ht->storage[i].keylen != 0) {
            //printf("Free: %p\n", ht->storage[i].key);
            free(ht->storage[i].key);
        }
    }

    // free storage array
    free(ht->storage);

    // free struct itself 
    free(ht);
}
