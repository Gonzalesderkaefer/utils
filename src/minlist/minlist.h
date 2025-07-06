#ifndef MIN_LIST_H
#define MIN_LIST_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct _ListStats {
    size_t capacity;
    size_t length;
    size_t elem_size; 
    void *storage;
} ListStats;


#define list_init(lst) ListStats __meta_datas__##lst

#define list_len(lst) __meta_datas__##lst.length

#define list_push(lst, elem) if (lst == NULL) { \
        lst = calloc(1, sizeof(*lst)); \
        if (lst != NULL) { \
            __meta_datas__##lst.capacity = 5; \
            __meta_datas__##lst.length = 0; \
            __meta_datas__##lst.elem_size = sizeof(*lst); \
            __meta_datas__##lst.storage = lst; \
            lst[__meta_datas__##lst.length++] = elem; \
        } \
    } else if (__meta_datas__##lst.capacity > __meta_datas__##lst.length) { \
        lst[__meta_datas__##lst.length++] = elem; \
        __meta_datas__##lst.storage = lst; \
    } else { \
        __meta_datas__##lst.storage = malloc((__meta_datas__##lst.capacity * 2) * __meta_datas__##lst.elem_size); \
        if (__meta_datas__##lst.storage != NULL) { \
            memcpy(__meta_datas__##lst.storage, lst, __meta_datas__##lst.length * __meta_datas__##lst.elem_size); \
            free(lst); \
            lst = __meta_datas__##lst.storage; \
            __meta_datas__##lst.storage = lst; \
            __meta_datas__##lst.capacity *= 2; \
            lst[__meta_datas__##lst.length++] = elem; \
        } else { \
            __meta_datas__##lst.storage = lst; \
        }\
    }

#define list_prealloc(lst, size) if (lst == NULL) { \
        lst = calloc(size, sizeof(*lst)); \
        if (lst != NULL) { \
            __meta_datas__##lst.capacity = size; \
            __meta_datas__##lst.length = 0; \
            __meta_datas__##lst.elem_size = sizeof(*lst); \
            __meta_datas__##lst.storage = lst; \
        } else {  \
        __meta_datas__##lst.storage = malloc((__meta_datas__##lst.capacity + size) * __meta_datas__##lst.elem_size); \
        if (__meta_datas__##lst.storage != NULL) { \
            memcpy(__meta_datas__##lst.storage, lst, __meta_datas__##lst.length * __meta_datas__##lst.elem_size); \
            free(lst); \
            lst = __meta_datas__##lst.storage; \
            __meta_datas__##lst.storage = lst; \
            __meta_datas__##lst.capacity += size; \
        } else { \
            __meta_datas__##lst.storage = lst; \
        }\
    }

#define list_stats(lst) __meta_datas__##lst

#endif // MIN_LIST_H
