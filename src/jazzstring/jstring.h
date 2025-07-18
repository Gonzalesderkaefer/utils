#ifndef J_STRING_H
#define J_STRING_H

// Libraries
#include <stddef.h>

/// Handle to a heap allocated String
///
/// Used as a handle to a String and
/// passed into functions by reference (as a pointer)
typedef struct {
    const char *string;
    const size_t length;
} String;

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



/// Initialize a String
///
/// This function initializes a [String]
///
/// Parameters:
///   - alloc: allcator function
///   - dealloc: deallocator function
///   - init_str: c-style string should be a literal
///
/// Returns:
///   Handle to a [String] or NULL on failure
String *string_init(const char *init_str, const Alloc alloc, const DeAlloc dealloc);






/// Push a string onto a string
///
/// Append/push a c-style string onto a [String] 
///
/// Parameters:
///   - string: handler to a string
///   - str: c-style string should be a literal
void string_push(String *string, const char *str);
#endif // J_STRING_H
