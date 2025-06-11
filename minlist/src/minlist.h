

#include <stdlib.h>
#include <stddef.h>
typedef struct _ListMetadata {
    size_t capacity;
    size_t length;
    size_t elem_size; 
    void *storage;

} ListMetadata;


#define list_init(lst) ListMetadata __meta_datas__##lst;

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
    } else  { \
        __meta_datas__##lst.storage = realloc(lst, (__meta_datas__##lst.capacity + 1) * __meta_datas__##lst.elem_size); \
        if (__meta_datas__##lst.storage != NULL) { \
            lst = __meta_datas__##lst.storage; \
            __meta_datas__##lst.storage = lst; \
            __meta_datas__##lst.capacity++; \
            lst[__meta_datas__##lst.length++] = elem; \
        } \
    }
