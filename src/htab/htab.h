#ifndef JAZZY_ALLOC_TYPES
#define JAZZY_ALLOC_TYPES

#include<stddef.h>


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Params:
/// - size_t: amount of bytes needed
typedef void *(*AllocFunc)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
///
/// Params:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void *(*ReAllocFunc)(void *, size_t);

/// This type represents functions that are used to allocate initialized memory
/// the function 'calloc' is of this type
///
/// Params:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void (*CalloFunc)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Params:
/// - void *: pointer to memory  to free
typedef void (*FreeFunc)(void *);




#endif // JAZZY_ALLOC_TYPES


#ifndef HTAB_H
#define HTAB_H

#include <sys/types.h>
#include <stdbool.h>


// Macros
#define INIT_SIZE 16


/// Handle to a hash table
///
/// This type is used as a handle to a 
/// hash_table and a pointer to this is used
/// to pass into functions and perform operations on.
typedef struct _Htab  Htab;

/// Initialize a dynamic list
///
/// This function initializes a hash table and allocates
/// the necessary memory according to [alloc]
///
/// Note:
/// Only stores the pointers not the values themselves.
///
/// Parameters:
///   - alloc: allocator function
///   - dealloc: free function
///
/// Returns:
///   A handle to a hash table or NULL on failure to allocate memory
Htab *htab_init(AllocFunc alloc, FreeFunc dealloc);



/// Insert a value and key into the hash table
///
/// This function adds a value and a key into the
/// hash table. Both value and key are copied so a key length
/// and value length have to be provided.
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_init`
///   - value: pointer to the value that will be added
///   - key: key that is associated with [value]
///   - value_len: length of the value that needs to be added
///   - keylen: length of the key
void htab_insert(Htab *htab, void *value, void *key, size_t value_len, size_t keylen);


/// Lookup a key in the hash table
///
/// This function returns a pointer to the value that is
/// associated with [key]. If the key exists.
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_init`
///   - key: key to lookup
///   - keylen: length of the key
///
/// Returns:
///   A pointer to the value if the key exists NULL
///   if it does not exist.
void *htab_lookup(Htab *htab, char *key, size_t keylen);


/// Free up memory used by the hash table
///
/// This function frees up memory that is used by the
/// hash table according to the dealloc function that
/// was provided when [htab] was initialized
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_init`
void htab_free(Htab *htab);

#endif // HTAB_H
