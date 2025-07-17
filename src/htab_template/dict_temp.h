#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h> 

#define INIT_SIZE 16


#define DictDef(valuetype) \
    typedef struct { \
        size_t keylen; \
        char *k; \
        valuetype v; \
        bool occupied; \
    } Pair_##valuetype; \
\
    typedef struct { \
        size_t length; \
        size_t capacity; \
        void *(*alloc)(size_t); \
        void (*dealloc)(void *); \
        Pair_##valuetype *storage; \
    } Dict_##valuetype

#define DictOf(valuetype) Dict_##valuetype
#define PairOf(valuetype) Pair_##valuetype

#define DictImpl(valuetype) \
    DictOf(valuetype) *dict_init_##valuetype(void *(*alloc)(size_t), void (*dealloc)(void *)); \
    uint64_t fnv1a_hash_##valuetype(const char *key, const size_t length); \
    void dict_insert_##valuetype(DictOf(valuetype) *htab, valuetype value, char *key, size_t keylen); \
    void grow_##valuetype(DictOf(valuetype) *htab); \
    void dict_free_##valuetype(DictOf(valuetype) *htab); \
    bool key_match_pair_##valuetype(const PairOf(valuetype) pair, const char *key, const size_t keylen); \
    valuetype *dict_lookup_##valuetype(DictOf(valuetype) *htab, char *key, size_t keylen); \
    \
    \
    \
    \
    \
    DictOf(valuetype) *dict_init_##valuetype(void *(*alloc)(size_t), void (*dealloc)(void *)) {\
        /* Allocators have to be specified */\
        if (alloc == NULL || dealloc == NULL) {\
         return NULL;\
        }\
        /* Allocate struct */\
        DictOf(valuetype) *new_table = alloc(sizeof(DictOf(valuetype)));\
        if (new_table == NULL) {\
            return NULL;\
        }\
    \
        /* Allocate storage */\
        PairOf(valuetype) *new_storage = alloc(sizeof(PairOf(valuetype)) * INIT_SIZE);\
        if (new_storage == NULL) {\
            dealloc(new_table);\
            return NULL;\
        }\
    \
        /* Zero out memory */\
        memset(new_storage, 0, sizeof(PairOf(valuetype)) * INIT_SIZE);\
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
    uint64_t fnv1a_hash_##valuetype(const char *key, const size_t length) {\
        uint64_t hash = 14695981039346656037LU;\
        for (int i = 0; i < length; ++i) {\
            hash ^= key[i];\
            hash *= 1099511628211;\
        }\
        return hash;\
    }\
    \
    \
    \
    void dict_insert_##valuetype(DictOf(valuetype) *htab, valuetype value, char *key, size_t keylen) {\
        /* Sanity check */\
        if (htab == NULL) {\
            return;\
        }\
    \
    \
        /* Calculate hash and index */\
        const uint64_t hash = fnv1a_hash_##valuetype(key, keylen);\
        const uint64_t index = hash % htab->capacity;\
    \
        /* If index is free. */\
        if (!htab->storage[index].occupied) {\
            /* Allocate new key */\
            char *new_key = htab->alloc(keylen);\
            if (new_key == NULL) {\
                return;\
            }\
            /* Copy key into buffer */\
            memcpy(new_key, key, keylen);\
\
            htab->storage[index].k = new_key;\
            htab->storage[index].v = value;\
            htab->storage[index].keylen = keylen;\
            htab->storage[index].occupied = true;\
        } else {\
            /* Find different index */\
            uint64_t other_index = (index + 1) % htab->capacity;\
            while (htab->storage[other_index].occupied) {\
                /* Check if we have to grow */\
                if (other_index == index) {\
                    grow_##valuetype(htab);\
                    /* This would be the ideal index for the new element */\
                    other_index = hash % htab->capacity;\
                } else {\
                    /* increment other_index */\
                    other_index = (other_index + 1) % htab->capacity;\
                }\
            }\
            /* Allocate new key */\
            char *new_key = htab->alloc(keylen);\
            if (new_key == NULL) {\
                return;\
            }\
            /* Copy key into buffer */\
            memcpy(new_key, key, keylen);\
\
            htab->storage[other_index].k = new_key;\
            htab->storage[other_index].v = value;\
            htab->storage[other_index].occupied = true;\
        }\
    }\
    \
    void dict_insert_after_grow_##valuetype(DictOf(valuetype) *htab, valuetype value, char *key, size_t keylen) {\
        /* Sanity check */\
        if (htab == NULL) {\
            return;\
        }\
    \
    \
        /* Calculate hash and index */\
        const uint64_t hash = fnv1a_hash_##valuetype(key, keylen);\
        const uint64_t index = hash % htab->capacity;\
    \
        /* If index is free. */\
        if (!htab->storage[index].occupied) {\
            htab->storage[index].k = key;\
            htab->storage[index].v = value;\
            htab->storage[index].keylen = keylen;\
            htab->storage[index].occupied = true;\
        } else {\
            /* Find different index */\
            uint64_t other_index = (index + 1) % htab->capacity;\
            while (htab->storage[other_index].occupied) {\
                /* Check if we have to grow */\
                if (other_index == index) {\
                    grow_##valuetype(htab);\
                    /* This would be the ideal index for the new element */\
                    other_index = hash % htab->capacity;\
                } else {\
                    /* increment other_index */\
                    other_index = (other_index + 1) % htab->capacity;\
                }\
            }\
            htab->storage[other_index].k = key;\
            htab->storage[other_index].v = value;\
            htab->storage[other_index].occupied = true;\
        }\
    }\
    \
    \
    void grow_##valuetype(DictOf(valuetype) *htab) {\
        /* Save storage */\
        PairOf(valuetype) *old_storage = htab->storage;\
        size_t old_cap = htab->capacity;\
        size_t new_alloc_size = sizeof(*htab->storage) * old_cap * 2;\
    \
        /* Allocate new storage */\
        void *new_storage = htab->alloc(new_alloc_size);\
        if (new_storage == NULL) {\
            return;\
        }\
        /* Overwrite with 0 */\
        memset(new_storage, 0, new_alloc_size);\
    \
        /* Assign new_storage to htab */\
        htab->storage = new_storage;\
        htab->capacity = old_cap * 2;\
    \
        for (uint64_t i = 0; i < old_cap; i++) {\
            const PairOf(valuetype) cur_pair = old_storage[i];\
            if (cur_pair.occupied) {\
                /* Insert Into new storage */\
                dict_insert_after_grow_##valuetype(htab, cur_pair.v, cur_pair.k, cur_pair.keylen);\
            }\
        }\
    \
        /* Cleanup old storage */\
        htab->dealloc(old_storage);\
    }\
    \
    void dict_free_##valuetype(DictOf(valuetype) *htab) {\
        /* Save pointer to deallocation function */\
        void (*dealloc)(void *) = htab->dealloc;\
    \
        /* Deallocate all keys */\
        for (int i = 0; i < htab->capacity; ++i) {\
            dealloc(htab->storage[i].k);\
        }\
    \
        /* Deallocate storage */\
        dealloc(htab->storage);\
    \
        /* Deallocate struct itself */\
        dealloc(htab);\
    }\
    bool key_match_pair_##valuetype(const PairOf(valuetype) pair, const char *key, const size_t keylen) {\
        /* Sanity check */ \
        if (key == NULL || keylen == 0) { \
            return false; \
        } \
    \
    \
        /* If key length is different, there's no point in keeping going. */ \
        if (pair.keylen != keylen) { \
            return false; \
        } \
        /* Compare the actual memory */ \
        if (memcmp(key, pair.k, keylen) == 0) { \
            return true; \
        } \
    \
        /* Keylen was same but memory was different */ \
        return false; \
    } \
    valuetype *dict_lookup_##valuetype(DictOf(valuetype) *htab, char *key, size_t keylen){ \
        /* Sanity check */ \
        if (htab == NULL) { \
            return NULL; \
        } \
        \
        /* Calculate hash and index */\
        const uint64_t hash = fnv1a_hash_##valuetype((char *)&key, keylen); \
        const uint64_t index = hash % htab->capacity; \
        \
        uint64_t other_index = index; \
        while (!key_match_pair_##valuetype(htab->storage[other_index], key, keylen)) { \
            other_index = (other_index + 1) % htab->capacity; \
            if (other_index == index) { \
                return NULL; \
            } \
        } \
        \
        /* Value was found otherwise we'd still be in the loop */ \
        return &htab->storage[other_index].v; \
    }
