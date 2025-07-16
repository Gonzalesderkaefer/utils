#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h> 

#define INIT_SIZE 16


#define HtabDef(keytype, valuetype) \
    typedef struct { \
        keytype k; \
        valuetype v; \
        bool occupied; \
    } Pair_##keytype##_##valuetype; \
\
    typedef struct { \
        size_t length; \
        size_t capacity; \
        void *(*alloc)(size_t); \
        void (*dealloc)(void *); \
        Pair_##keytype##_##valuetype *storage; \
    } Htab_##keytype##_##valuetype

#define HtabOf(keytype, valuetype) Htab_##keytype##_##valuetype
#define PairOf(keytype, valuetype) Pair_##keytype##_##valuetype

#define HtabImpl(keytype, valuetype) \
    HtabOf(keytype, valuetype) *htab_init_##keytype##_##valuetype(void *(*alloc)(size_t), void (*dealloc)(void *)); \
    uint64_t fnv1a_hash_##keytype##_##valuetype(const char *key, const size_t length); \
    void htab_insert_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab, valuetype value, keytype key); \
    void grow_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab); \
    void htab_free_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab); \
    bool key_match_pair_##keytype##_##valuetype(const PairOf(keytype, valuetype) pair, const char *key, const size_t keylen); \
    valuetype *htab_lookup_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab, keytype key); \
    \
    \
    \
    \
    \
    HtabOf(keytype, valuetype) *htab_init_##keytype##_##valuetype(void *(*alloc)(size_t), void (*dealloc)(void *)) {\
        /* Allocators have to be specified */\
        if (alloc == NULL || dealloc == NULL) {\
         return NULL;\
        }\
        /* Allocate struct */\
        HtabOf(keytype, valuetype) *new_table = alloc(sizeof(HtabOf(keytype, valuetype)));\
        if (new_table == NULL) {\
            return NULL;\
        }\
    \
        /* Allocate storage */\
        PairOf(keytype, valuetype) *new_storage = alloc(sizeof(PairOf(keytype, valuetype)) * INIT_SIZE);\
        if (new_storage == NULL) {\
            dealloc(new_table);\
            return NULL;\
        }\
    \
        /* Zero out memory */\
        memset(new_storage, 0, sizeof(PairOf(keytype, valuetype)) * INIT_SIZE);\
    \
        /* Assign fields */\
        new_table->alloc = alloc;\
        new_table->dealloc = dealloc;\
        new_table->storage = (void *)new_storage;\
        new_table->capacity = INIT_SIZE;\
        new_table->length = 0;\
    \
    \
        return new_table;\
    }\
    \
    \
    /* NOTE: See (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) */\
    uint64_t fnv1a_hash_##keytype##_##valuetype(const char *key, const size_t length) {\
        uint64_t hash = 14695981039346656037LU;\
        for (valuetype i = 0; i < length; ++i) {\
            hash ^= key[i];\
            hash *= 1099511628211;\
        }\
        return hash;\
    }\
    \
    \
    \
    void htab_insert_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab, valuetype value, keytype key) {\
        /* Sanity check */\
        if (htab == NULL) {\
            return;\
        }\
    \
    \
        /* Calculate hash and index */\
        const uint64_t hash = fnv1a_hash_##keytype##_##valuetype((char *)&key, sizeof(key));\
        const uint64_t index = hash % htab->capacity;\
    \
        /* If index is free. */\
        if (!htab->storage[index].occupied) {\
            htab->storage[index].k = key;\
            htab->storage[index].v = value;\
            htab->storage[index].occupied = true;\
        } else {\
            /* Find different index */\
            uint64_t other_index = (index + 1) % htab->capacity;\
            while (htab->storage[other_index].occupied) {\
                if (other_index == index) { \
                    grow_##keytype##_##valuetype(htab); \
                } \
                /* increment other_index */\
                other_index = (other_index + 1) % htab->capacity;\
            }\
            htab->storage[other_index].k = key;\
            htab->storage[other_index].v = value;\
            htab->storage[other_index].occupied = true;\
        }\
    }\
    \
    \
    \
    void grow_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab) {\
        /* save storage */\
        PairOf(keytype, valuetype) *old_storage = htab->storage;\
        size_t old_cap = htab->capacity;\
    \
        /* Allocate new storage */\
        void *new_storage = htab->alloc(sizeof(*htab->storage) * old_cap * 2);\
        if (new_storage == NULL) {\
            return;\
        }\
        /* Overwrite with 0 */\
        memset(new_storage, 0, old_cap * 2);\
    \
        /* Assign new_storage to htab */\
        htab->storage = new_storage;\
        htab->capacity = old_cap * 2;\
    \
        for (uint64_t i = 0; i < old_cap; i++) {\
            const PairOf(keytype, valuetype) cur_pair = old_storage[i];\
    \
            /* Insert valuetypeo new storage */\
            htab_insert_##keytype##_##valuetype( htab, cur_pair.v, cur_pair.k);\
        }\
    \
        /* Cleanup old storage */\
        htab->dealloc(old_storage);\
    }\
    \
    void htab_free_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab) {\
        /* Save povaluetypeer to deallocation function */\
        void (*dealloc)(void *) = htab->dealloc;\
    \
        /* Deallocate storage */\
        dealloc(htab->storage);\
    \
        /* Deallocate struct itself */\
        dealloc(htab);\
    }\
    bool key_match_pair_##keytype##_##valuetype(const PairOf(keytype, valuetype) pair, const char *key, const size_t keylen) {\
        /* Sanity check */ \
        if (key == NULL || keylen == 0) { \
            return false; \
        } \
    \
    \
        /* If key length is different, there's no point in keeping going. */ \
        if (sizeof(pair.k) != keylen) { \
            return false; \
        } \
        /* Compare the actual memory */ \
        if (memcmp(key, &pair.k, keylen) == 0) { \
            return true; \
        } \
    \
        /* Keylen was same but memory was different */ \
        return false; \
    } \
    valuetype *htab_lookup_##keytype##_##valuetype(HtabOf(keytype, valuetype) *htab, keytype key){ \
        /* Sanity check */ \
        if (htab == NULL) { \
            return NULL; \
        } \
        \
        /* Calculate hash and index */\
        const uint64_t hash = fnv1a_hash_double_int((char *)&key, sizeof(key)); \
        const uint64_t index = hash % htab->capacity; \
        \
        uint64_t other_index = index; \
        while (!key_match_pair_double_int(htab->storage[other_index], (char *)&key, sizeof(key))) { \
            other_index = (other_index + 1) % htab->capacity; \
            if (other_index == index) { \
                return NULL; \
            } \
        } \
        \
        /* Value was found otherwise we'd still be in the loop */ \
        return &htab->storage[other_index].v; \
    }

