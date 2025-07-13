#ifndef MIN_LIST_H
#define MIN_LIST_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct _ListStats {
    size_t capacity;
    size_t length;
    size_t elem_size; 
    void *(*alloc)(size_t);
    void (*dealloc)(void *);
    void *storage;
} ListStats;

/*********************************** Macros ***********************************/

#define list_init(lst) \
    ListStats list_stats##lst = (ListStats) { .capacity = 0, .length = 0, .elem_size = sizeof(*lst), .storage = lst, .alloc = malloc, .dealloc = free }; \
    ListStats *list_handle##lst = &list_stats##lst

#define list_push(lst, elem) \
    grow_list_stat(list_handle##lst); \
    lst = list_handle##lst->storage; \
    lst[list_handle##lst->length++] = elem

#define list_len(lst) \
    list_stats##lst.length

#define list_action(lst, action) \
    for (int i = 0; i < list_stats##lst.length; ++i) action(lst[i])

#define list_free(lst) \
    list_handle##lst->dealloc(list_handle##lst->storage);

#define list_stat(lst) list_handle##lst

#define list_from_stats(lstname, stats) \
    lstname = stats->storage; \
    ListStats *list_handle##lstname = stats; \


/******************************** Functions ***********************************/

static void grow_list_stat(ListStats *lststat) {
    if (lststat->length < lststat->capacity) {// If everything is okay
        return;
    }
    if (lststat->storage == NULL) { // Needs to be initialized
        lststat->storage = lststat->alloc(10 * lststat->elem_size);
        lststat->capacity = 10;
        if (lststat->storage == NULL) { // Error checking
            return;
        }
    } else {
        // Allocate new storage
        void *new_storage = lststat->alloc(lststat->capacity * 2 * lststat->elem_size);
        if (new_storage == NULL) {
            return;
        }
        // Copy old storage into new storage
        memcpy(new_storage, lststat->storage, lststat->length);

        // free old storage 
        lststat->dealloc(lststat->storage);

        // Update capacity
        lststat->capacity *= 2;

        // Reassign new storage
        lststat->storage = new_storage;
    }
}
#endif // MIN_LIST_H
