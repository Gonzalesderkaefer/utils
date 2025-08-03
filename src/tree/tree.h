#ifndef JAZZY_ALLOC_H
#define JAZZY_ALLOC_H

// Libraries
#include <stddef.h>
#include <stdlib.h>

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
static void __malloc_free(void *context, void *ptr) { free(ptr); }
static void *__malloc_realloc(void *context, void *ptr, size_t new_size) { return realloc(ptr, new_size); }

/// Macro for the default allocator
#define default_allocator create_allocator(NULL, __malloc_alloc, __malloc_free, __malloc_realloc)


/* The following part is seperate from above */

/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
typedef void *(*AllocFunc)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
typedef void *(*ReAllocFunc)(void *, size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
typedef void (*FreeFunc)(void *);



#endif // JAZZY_ALLOC_H



#ifndef JAZZY_TREE_H
#define JAZZY_TREE_H



/// This type represents functions that are used to compare to
/// blocks of memory the function 'memcmp' is of this type
typedef int (*Comparator)(const void *, const void *, size_t);

/// A handle to a tree
///
/// This type represents a handle to a tree. A pointer to this is passed
/// into function that perform operations on it
typedef struct _Tree Tree;

/// Initialize a tree
///
/// This function initializes a tree. I.e allocates memory according to [alloc]
/// elements in this tree are copied which is why a elem_size should be provided
///
/// Parameters:
///   - elem_size: size of the elements that are stored in the tree
///   - alloc: memory allocator
///   - dealloc: memory free function
///   - comp: function used to compare two values
///
/// Returns:
///   A pointer to a tree or NULL if the memory allocation fails
Tree *tree_init(const size_t elem_size, AllocFunc alloc, FreeFunc dealloc, Comparator comp);

/// Insert a value into a tree
///
/// This function inserts a value into the tree. The value is copied and should
/// have the same size that was specified in `tree_init`. The behaivor for different
/// sized values is NOT DEFINED.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be inserted
void tree_insert(Tree *tree, const void *value);

/// Look up a value in a tree
///
/// This function looksup values in the tree and returns a pointer to the data.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be looked up
///
/// Returns:
///   a pointer to the value if it was found and NULL other wise.
const void *tree_lookup(const Tree *tree, const void *value);

/// Delete a value from the tree 
///
/// This function deletes a value from the tree if it is found.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be deleted
void tree_delete(Tree *tree, const void *value);

/// Free the entire tree
///
/// This function frees the tree according to [dealloc] which was specified
/// in `tree_init`. The tree should not be used after it has been freed.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
void tree_free(Tree *tree);





#endif // JAZZY_TREE_H
