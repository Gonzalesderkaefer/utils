#ifndef JAZZY_ALLOCATOR
#define JAZZY_ALLOCATOR

// Libraries
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/// This struct exists to encapsulate an Allocator.
/// It is used to initialize Data structures etc.
/// Usually passed by value into an initializer function.
typedef struct _Allocator {
    /// Handle to context
    void *context;

    /// Params:
    /// - void * -> Pointer to context
    /// - size_t -> amount of bytes neede
    void *(*alloc)(void *, size_t);

    /// Params:
    /// - void * -> Pointer to context
    /// - void * -> Pointer to memory
    void (*dealloc)(void *, void *);

    /// Params:
    /// - void * -> Pointer to context
    /// - void * -> Pointer to memory
    /// - size_t -> new size
    void *(*realloc)(void *, void *, size_t);
} Allocator;


/// This macro creates an Allocator. Use only if desired.
#define create_allocator(cont, allocat, deallocat, reallocat) \
    (Allocator) { \
        .context = cont, \
        .alloc = allocat, \
        .dealloc = deallocat, \
        .realloc = reallocat, \
    }


/// This allocator just encapsulates
/// malloc, free and realloc
static void *__malloc_alloc(void *context, size_t size) { return malloc(size); }
static void __malloc_free(void *context, void *ptr) { return free(ptr); }
static void *__malloc_realloc(void *context, void *ptr, size_t new_size) { return realloc(ptr, new_size); }


#define default_allocator create_allocator(NULL, __malloc_alloc, __malloc_free, __malloc_realloc)


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
typedef void *(*AllocFunc)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
typedef void *(*ReAllocFunc)(void *, size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
typedef void (*FreeFunc)(void *);


#endif // JAZZY_ALLOCATOR


#ifndef DYNLIST_H
#define DYNLIST_H


/// Handle to a dynamic list
///
/// This type is used as a handle to a 
/// dynamic list and a pointer to this is used
/// to pass into functions and perform operations on.
typedef struct _DynList DynList;


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
DynList *dynlist_init(const Allocator allocator, const size_t elem_size);




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
DynList *dynlist_init_def(const size_t elem_size);

/// Clean up a dynamic list
///
/// This function frees the memory that is used by this
/// dynamic list
///
/// Paramters:
///   - list: handle to a dynamic list that was returned by `dynlist_init()`
void dynlist_free(DynList *list);


/// Get the length of the list
///
/// This function returns the length of the 
/// list as
/// 
/// Parameters:
///   - list: handle to a dynamic list that was returned by `dynlist_init()`
/// Returns:
///   The number of elements in the list
size_t dynlinst_len(const DynList *list);

/// Push a value into the list
///
/// This function pushes pointer to [value] into the
/// dynamic list. The value is not copied.
///
/// Parameters:
///   - list: handle to a dynamic list that was returned by `dynlist_init()`
///   - value: a reference(pointer) to a value that needs to be pushed
void dynlist_push(DynList *list, const void *value);


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
void *dynlist_at(const DynList *list, const uint64_t index);


/// Get the contents of [list]
///
/// This function returns a pointer to the underlying 
/// storage of the list which is NULL terminated
///
/// Parameters:
///   - list: handle to a list that was returned by `dynlist_init()`
///
/// Return:
///   Pointer to the underlying storage of [list].
void *dynlist_arr(const DynList *list) {



/// Perform [action] on all elements
///
/// This function performs an action on all elements in the
/// list from first to last and can mutate the values.
///
/// Parameters:
///   - list: handle to a list that was returned by `dynlist_init()`
///   - action: function that operates on the values
void dynlist_action(const DynList *list, void (*action)(void *));

#endif // DYNLIST_H
