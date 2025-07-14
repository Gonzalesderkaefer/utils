
#include <sys/types.h>
#include <stddef.h>
#include <string.h>



#define list_type(type) \
    struct { \
        size_t capacity; \
        size_t length; \
        void *(*alloc)(size_t); \
        void (*dealloc)(void *); \
        type *storage; \
    }

#define list_init(list, allocat, deallocat) \
    list.alloc = allocat; list.dealloc = deallocat

#define list_push(list, new_value) \
    grow_list((void **) &list.storage, sizeof(*list.storage), &list.capacity, &list.length, list.alloc, list.dealloc); \
    list.storage[list.length++] = new_value






static void grow_list(void **storage, size_t elem_size, size_t *cap, size_t *len,
        void *(*alloc)(size_t), void (*dealloc)(void *))  {
    // Sanity check
    if (elem_size == 0 || cap == NULL || len == NULL || storage == NULL) {
        return;
    }

    // No need to grow
    if (*cap > *len && *storage != NULL) {
        return;
    }

    if (*storage == NULL) { // Needs to be initialized
        *storage = alloc(10 * elem_size);
        *cap = 10;
        if (*storage == NULL) {
            return;
        }
    } else {
        // Allocate new storage
        void *new_storage = alloc(*cap * 2 * elem_size);
        if (new_storage == NULL) {
            return;
        }
        // Copy old storage into new storage
        memcpy(new_storage, *storage, *len * elem_size);

        // free old storage 
        dealloc(*storage);

        // Update capacity
        *cap *= 2;

        // Reassign new storage
        *storage = new_storage;
    }
}
