#ifndef TREE_H
#define TREE_H
// Libraries
#include <sys/types.h>

/// Handle to a Tree
///
/// This struct is a handle to a Tree and is
/// passed around as a pointer.
typedef struct _Tree Tree;

/// Allocator functions similar to free
///
/// This type represents a function that allocates memory.
/// malloc() would have this type.
///
/// Parameters:
///   - size: amount of memory that needs to be allocated
///
/// Returns:
///   A pointer to the allocated memory on success, NULL on failure
typedef void *(*Alloc)(size_t size);

/// Deallocator functions similar to free
///
/// This type represents a function that frees memory.
/// free() would have this type.
///
/// Parameters:
///   - ptr: pointer to memory that needs to be freed
typedef void (*DeAlloc)(void *ptr);


/// Comparator function
///
/// This type represents a compare function. That compares two
/// values.
///
/// Parameters:
///   s1 - first value
///   s2 - second value
///   size - amount of bytes that need to be compared
///
/// Returns:
///   - an int < 0 if s1 < s2
///   - an int = 0 if s1 == s2
///   - an int > 0 if s1 > s2
typedef int (*Comparator)(const void *s1, const void *s2, const size_t size);



/// Initialize a Tree
///
/// This function allcoates memory for a Tree according to
/// [alloc]
///
/// Parameters:
///   alloc - allocator function (like [malloc()])
///   dealloc - free function (like [free()])
///   compare - compare function.
///   element_size - size of the elements that are to be stored used for [compare]
///
/// Returns:
///   Handle/pointer to a [Tree] on success, NULL [alloc] at any point returns NULL;
Tree *tree_init(const Alloc alloc, const DeAlloc dealloc, const Comparator compare, const size_t elem_size);



/// Insert a value into the tree
///
/// This function insert [value] into the tree. It's assumed that
/// the size of the memory pointed to by [value] has the same size
/// as [elem_size] that was specified in [tree_init]. The memory will 
/// be copied.
///
/// Parameters:
///   - tree:  a handle to a [Tree]
///   - value: value to be inserted
void tree_insert(Tree *tree, void *value);



#endif // TREE_H
