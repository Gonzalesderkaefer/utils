#ifndef LIN_ALLOC_H
#define LIN_ALLOC_H
// Libraries
#include <stddef.h>

/// This struct represents a handle to a
/// linear allocator and is accesed as a pointer
typedef struct _LinAlloc LinAlloc;

/// This function creates a handle to a linear allocator
/// that has to be freed after its not used anymore
LinAlloc *linear_init();


/// This function alloctaes a chunk of memory of size
/// in the linear allocator.
void *lalloc(LinAlloc *linear_alloc, size_t size);



/// This function frees the entire linear allocator
void linear_free(LinAlloc *linear_alloc);
#endif // LIN_ALLOC_H
