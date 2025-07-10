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


#define list_init(lst) ListStats list_stats##lst

#define list_len(lst) list_stats##lst.length

#define list_push(lst, elem) if (lst == NULL) { \
        lst = calloc(1, sizeof(*lst)); \
        if (lst != NULL) { \
            list_stats##lst.capacity = 5; \
            list_stats##lst.length = 0; \
            list_stats##lst.elem_size = sizeof(*lst); \
            list_stats##lst.storage = lst; \
            lst[list_stats##lst.length++] = elem; \
        } \
    } else if (list_stats##lst.capacity > list_stats##lst.length) { \
        lst[list_stats##lst.length++] = elem; \
        list_stats##lst.storage = lst; \
    } else { \
        list_stats##lst.storage = malloc((list_stats##lst.capacity * 2) * list_stats##lst.elem_size); \
        if (list_stats##lst.storage != NULL) { \
            memcpy(list_stats##lst.storage, lst, list_stats##lst.length * list_stats##lst.elem_size); \
            free(lst); \
            lst = list_stats##lst.storage; \
            list_stats##lst.storage = lst; \
            list_stats##lst.capacity *= 2; \
            lst[list_stats##lst.length++] = elem; \
        } else { \
            list_stats##lst.storage = lst; \
        }\
    }

#define list_prealloc(lst, size) if (lst == NULL) { \
        lst = calloc(size, sizeof(*lst)); \
        if (lst != NULL) { \
            list_stats##lst.capacity = size; \
            list_stats##lst.length = 0; \
            list_stats##lst.elem_size = sizeof(*lst); \
            list_stats##lst.storage = lst; \
        } else {  \
        list_stats##lst.storage = malloc((list_stats##lst.capacity + size) * list_stats##lst.elem_size); \
        if (list_stats##lst.storage != NULL) { \
            memcpy(list_stats##lst.storage, lst, list_stats##lst.length * list_stats##lst.elem_size); \
            free(lst); \
            lst = list_stats##lst.storage; \
            list_stats##lst.storage = lst; \
            list_stats##lst.capacity += size; \
        } else { \
            list_stats##lst.storage = lst; \
        }\
    }

#define list_stats(lst) list_stats##lst

#define list_action(lst, fn) \
    for (int i = 0; i < list_stats##lst.length; ++i) { \
        fn(lst[i]); \
    } \

#define list_from_stats(stats, lst) \
    lst = stats.storage; \
    ListStats list_stats##lst = stats;






#endif // MIN_LIST_H
