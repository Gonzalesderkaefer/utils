// Header file
#include "reflist.h"
#include <stdint.h>
#include <string.h>
#include <stddef.h>



/// Handle to a dynamic list
///
/// This type is used as a handle to a 
/// dynamic list and a pointer to this is used
/// to pass into functions and perform operations on.
typedef struct _RefList {
    size_t cap;
    size_t len;
    void **storage;
    Allocator allocator;
} RefList;


/// Initialize a dynamic list
///
/// This function initializes a dynamic list.
/// An Allocator is used to define how the memory for
/// this list is supposed to be managed. If you don't
/// know what allocator to use, just use '&default_allocator'
///
/// Note:
/// This list only stores the pointers not the values themselves.
///
/// Parameters:
///   - allocator: Memory allocator to define how memory has to
///     be allocated. Use '&default_allocator' by default if you don't know
///     what to use.
///
/// Returns:
///   A handle to a dynamic list or NULL on failure to allocate memory
RefList *reflist_init(const Allocator allocator) {
    // Allocate dynamic List
    RefList *new_list = allocator.alloc(allocator.context, sizeof(RefList));
    if (new_list == NULL) {
        return NULL;
    }

    // Assign fields
    new_list->allocator = allocator;
    new_list->storage = NULL;
    new_list->cap = 0;
    new_list->len = 0;

    return new_list;
}


/// Initialize a dynamic list
///
/// This function initializes a dynamic list with a default
/// Allocator
///
/// Note:
/// This list only stores the pointers not the values themselves.
///
///
/// Returns:
///   A handle to a dynamic list or NULL on failure to allocate memory
RefList *reflist_init_def() {
    return reflist_init(default_allocator);
}



/// Clean up a dynamic list
///
/// This function frees the memory that is used by this
/// dynamic list
///
/// Paramters:
///   - list: handle to a dynamic list that was returned by `reflist_init()`
void reflist_free(RefList *list) {
    // Sanity check
    if (list == NULL) {
        return;
    }

    // Save allcoator
    Allocator alloc = list->allocator;

    // Free list
    alloc.dealloc(alloc.context, list);
}



/// Push a value into the list
///
/// This function pushes pointer to [value] into the
/// dynamic list. The value is not copied.
///
/// Parameters:
///   - list: handle to a dynamic list that was returned by `reflist_init()`
///   - value: a reference(pointer) to a value that needs to be pushed
void reflist_push(RefList *list, const void *value) {
    // Check if list storage needs to be initialized
    if (list->storage == NULL) {
        list->storage = list->allocator.alloc(list->allocator.context, sizeof(void *) * 10);
        if (list->storage == NULL) {
            return;
        }
        list->cap = 10;
        list->storage[list->len++] = (void *)value;
    } else if (list->cap <= list->len) {
        // Allocate storage for new storage (its twice the size as before)
        void **new_storage = list->allocator.alloc(list->allocator.context, sizeof(void *) * list->cap * 2);
        if (new_storage == NULL) {
            return;
        }

        // Copy old values into new buffer 
        memcpy(new_storage, list->storage, sizeof(void *) * list->cap);

        // Assign new value
        new_storage[list->len++] = (void *)value;

        // Delete old storage
        list->allocator.dealloc(list->allocator.context, list->storage);

        // Assign new storage to list
        list->storage = new_storage;
    } else {
        // Just append new value
        list->storage[list->len++] = (void *)value;
    }
}


/// Get the value at [index]
///
/// This function gives the value that is located at [index]
///
/// Parameters:
///   - list: handle to a list that was returned by `reflist_init()`
///   - index: index to lookup at
///
/// Returns:
///   A pointer to the value or NULL if the index is invalid
void *reflist_at(const RefList *list, const uint64_t index) {
    if (index >= list->len) {
        return NULL;
    }

    // Return the value
    return list->storage[index];
}

/// Perform [action] on all elements
///
/// This function performs an action on all elements in the
/// list from first to last and can mutate the values.
///
/// Parameters:
///   - list: handle to a list that was returned by `reflist_init()`
///   - action: function that operates on the values
void reflist_action(const RefList *list, void (*action)(void *)) {
    for (uint64_t i = 0; i < list->len; ++i) {
        action(list->storage[i]);
    }
}
