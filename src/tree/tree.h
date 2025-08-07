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


/// Special handle to a tree
///
/// This type represents a special handle to a tree. This pointer actually
/// points to a temporary buffer that is used to store values that are
/// passed to the tree.
typedef void *SpecialTree;



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


/// Initialize a Tree with a value buffer
///
/// This function works like `tree_init` but the pointer that is returned by
/// `tree_init` lies sizeof(Tree *) bytes behind the pointer retuned by this
/// function. This function is actually not meant to be used normally it is a
/// helper for a future macro wrapper for this data structure
///
/// Parameters:
///   - elem_size: size of the elements that are stored in the tree
///   - alloc: memory allocator
///   - dealloc: memory free function
///   - comp: function used to compare two values
///
/// Returns:
///   A pointer to a value buffer or NULL if the memory allocation fails
SpecialTree tree_init_special(const size_t elem_size, AllocFunc alloc, FreeFunc dealloc, Comparator comp);

/// Get a tree handle from a special handle
///
/// This function returns a normal tree handle from a SpecialHandle
///
/// Parameters:
///   - handle: special tree handle that was returned by `tree_init_w_buf` 
///
/// Returns:
///   - A normal tree handle
Tree *tree_from_handle(SpecialTree handle);

/// Insert a value into a tree
///
/// This function inserts a value into the tree. The value is copied and should
/// have the same size that was specified in `tree_init`. The behaivor for different
/// sized values is NOT DEFINED.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be inserted
///   - value_size: size of the value that will inserted into the tree
void tree_insert(Tree *tree, const void *value, const size_t value_size);

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


/// Free the entire tree
///
/// This function frees the tree according to [dealloc] which was specified
/// in `tree_init_special`. The tree should not be used after it has been freed.
///
/// Parameters:
///   - handle: special handle to a tree that was returned by `tree_init_special`
void tree_free_special(SpecialTree handle);



/// Put a value into a Tree
///
/// This macro inserts a value into a tree. The handle
/// to the tree has to be pointer to the type of [value] and should
/// be initially be set to NULL. This macro mutates the value the pointer
/// that is provided.
///
/// Parameters:
///   - tree: a pointer to a type of [value]
///   - value: value that needs to be inserted
#define tree_put(tree, value) \
    if (tree == NULL) { \
        tree = tree_init_special(sizeof(*tree), malloc, free, memcmp); \
    } \
    if (tree != NULL) { \
        *tree = value; \
        tree_insert(tree_from_handle(tree), tree, (sizeof(*tree))); \
    }

/// Get a value from the tree
///
/// This macro gets [value] from the [tree] provided and stores
/// it in the provided [variable]. The value in the variable should not
/// be mutated. If [tree] is NULL [varaible] will also be NULL
///
/// Parameters:
///   - variable: the variable which stores a pointer to the value in the tree.
///   - tree: a pointer to a type of [value]
///   - value: the value to lookup
#define tree_get(variable, tree, value) \
    *tree = value; \
    variable = (tree_lookup(tree_from_handle(tree), tree)); \


/// Delete a tree
///
/// This macro frees/deletes a tree that is accessed through a special
/// handle.
///
/// Parameters:
///   - tree: a pointer to a type of [value]
#define tree_del(tree) \
    tree_free_special(tree);




#endif // JAZZY_TREE_H
