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
#define INIT_SIZE 4


/// These macros exist to make using the hash table slightly easier
#define htab_put(table, val_type, key_type, value, key) \
    htab_insert(table, (val_type[1]){value}, (key_type[1]){key}, sizeof(val_type), sizeof(key_type))

#define htab_get(table, key_type, key) \
    htab_lookup(table, (key_type[1]){key}, sizeof(key));



/// Handle to a hash table
///
/// This type is used as a handle to a 
/// hash_table and a pointer to this is used
/// to pass into functions and perform operations on.
typedef struct _Htab  Htab;

/// Initialize a hash table
///
/// This function initializes a hash table and allocates
/// the necessary memory according to [alloc]
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
void *htab_lookup(Htab *htab, void *key, size_t keylen);


/// Free up memory used by the hash table
///
/// This function frees up memory that is used by the
/// hash table according to the dealloc function that
/// was provided when [htab] was initialized
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_init`
void htab_free(Htab *htab);


/// These macros and typedefs basically make it so that
/// the hash table can be used as a dictionary
/// So the key is a null terminated string and the
/// value has any type


/// Handle to a Dictionary
///
/// This type is used as a handle to a 
/// dictionary and a pointer to this is used
/// to pass into functions and perform operations on.
typedef Htab Dict;

/// Initialize a dictionary
///
/// This macro initializes a dictionary and allocates
/// the necessary memory according to [alloc]
///
/// Parameters:
///   - alloc: allocator function
///   - dealloc: free function
///
/// Returns:
///   A handle to a hash table or NULL on failure to allocate memory
#define dict_init(alloc, dealloc) htab_init(alloc, dealloc)




/// Free up memory used by the dictionary
///
/// This macro frees up memory that is used by the
/// hash table according to the dealloc function that
/// was provided when [dict] was initialized
///
/// Parameters:
///   - dict: handle to a hashtable that was returned by `dict_init`
#define dict_free(dict) htab_free(dict)



/// Insert a value into a dictionary
///
/// This macro inserts a value that is associated with
/// a string into the dictionary.
///
/// Parameters:
///   - dict: handle to a hashtable that was returned by `dict_init`
///   - type: type of the value that needs to be inserted
///   - value: the value itself has to be of type [type]
///   - key_string: A NULL terminated c-string
#define dict_put(dict, type, value, key_string) \
    htab_insert(dict, (type[1]){value}, key_string, sizeof(type), strlen(key_string) + 1)

/// Get a value from a dictionary
///
/// This macro returns a pointer to a the value associated with
/// with [key] if it exists.
///
/// Parameters:
///   - key: A NULL terminated c-string
///
/// Returns:
///   A pointer to the value if the key exists NULL
///   if it does not exist.
#define dict_get(dict, key) \
    htab_lookup(dict, key, strlen(key) + 1)



#endif // HTAB_H
