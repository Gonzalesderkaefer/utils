// Header file
#include "dynlist.h"
#include <stdint.h>
#include <string.h>
#include <stddef.h>



/// Handle to a dynamic list
///
/// This type is used as a handle to a 
/// dynamic list and a pointer to this is used
/// to pass into functions and perform operations on.
typedef struct _DynList {
    size_t cap;
    size_t len;
    size_t elem_size;
    void *storage;
    Allocator allocator;
} DynList;


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
DynList *dynlist_init(const Allocator allocator, const size_t elem_size) {
    // Allocate dynamic List
    DynList *new_list = allocator.alloc(allocator.context, sizeof(DynList));
    if (new_list == NULL) {
        return NULL;
    }

    // Assign fields
    new_list->allocator = allocator;
    new_list->storage = NULL;
    new_list->cap = 0;
    new_list->len = 0;
    new_list->elem_size = elem_size;

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
/// Parameters:
///   - allocator: Memory allocator to define how memory has to
///     be allocated. Use '&default_allocator' by default if you don't know
///     what to use.
///
/// Returns:
///   A handle to a dynamic list or NULL on failure to allocate memory
DynList *dynlist_init_def(const size_t elem_size) {
    return dynlist_init(default_allocator, elem_size);
}

/// Clean up a dynamic list
///
/// This function frees the memory that is used by this
/// dynamic list
///
/// Paramters:
///   - list: handle to a dynamic list that was returned by `dynlist_init()`
void dynlist_free(DynList *list) {
    // Sanity check
    if (list == NULL) {
        return;
    }

    // Save allcoator
    Allocator alloc = list->allocator;

    // Free Storage
    alloc.dealloc(alloc.context, list->storage);

    // Free list
    alloc.dealloc(alloc.context, list);
}



/// Push a value into the list
///
/// This function pushes pointer to [value] into the
/// dynamic list. The value is not copied.
///
/// Parameters:
///   - list: handle to a dynamic list that was returned by `dynlist_init()`
///   - value: a reference(pointer) to a value that needs to be pushed
void dynlist_push(DynList *list, const void *value) {
    // Check if list storage needs to be initialized
    if (list->storage == NULL) {
        list->storage = list->allocator.alloc(list->allocator.context, list->elem_size * 10);
        if (list->storage == NULL) {
            return;
        }
        list->cap = 10;

        // Get pointer to insert at.
        void *ptr_to_insert = list->storage + list->elem_size * list->len;
        // Copy/Append value to the buffer
        memcpy(ptr_to_insert, value, list->elem_size);
        // List is now longer
        list->len++;

    } else if (list->cap <= list->len) {
        // Allocate storage for new storage (its twice the size as before)
        void *new_storage = list->allocator.alloc(list->allocator.context, list->elem_size  * list->cap * 2);
        if (new_storage == NULL) {
            return;
        }

        // Copy old values into new buffer 
        memcpy(new_storage, list->storage, list->elem_size * list->cap);

        // Get pointer to insert at.
        void *ptr_to_insert = new_storage + list->elem_size * list->len;
        // Copy/Append value to the buffer
        memcpy(ptr_to_insert, value, list->elem_size);
        // List is now longer
        list->len++;

        // Delete old storage
        list->allocator.dealloc(list->allocator.context, list->storage);

        // Assign new storage to list
        list->storage = new_storage;
    } else {
        // Get pointer to insert at.
        void *ptr_to_insert = list->storage + list->elem_size * list->len;
        // Copy/Append value to the buffer
        memcpy(ptr_to_insert, value, list->elem_size);
        // List is now longer
        list->len++;
    }
}


/// Get the value at [index]
///
/// This function gives the value that is located at [index]
///
/// Parameters:
///   - list: handle to a list that was returned by `dynlist_init()`
///   - index: index to lookup at
///
/// Returns:
///   A pointer to the value or NULL if the index is invalid
void *dynlist_at(const DynList *list, const uint64_t index) {
    if (index >= list->len) {
        return NULL;
    }

    return list->storage + list->elem_size * index;
}

/// Perform [action] on all elements
///
/// This function performs an action on all elements in the
/// list from first to last and can mutate the values.
///
/// Parameters:
///   - list: handle to a list that was returned by `dynlist_init()`
///   - action: function that operates on the values
void dynlist_action(const DynList *list, void (*action)(void *)) {
    for (uint64_t i = 0; i < list->len; ++i) {
        action(list->storage + list->elem_size * i);
    }
}
