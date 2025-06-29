#ifndef LIN_ALLOC_H
#define LIN_ALLOC_H
// Libraries
#include <stddef.h>

/// This struct represents a region of memeory
typedef struct _Block Block;

/// This struct represents a handle to a
/// linear allocator and is passed around as a pointer
/// You have to init it with:
/// ```
/// LinAlloc glob_mem = {0};
/// ```
typedef struct _LinAlloc {
    Block *first;
    Block *current;
} LinAlloc;

/// This function alloctaes a chunk of memory of size
/// in the linear allocator.
void *lalloc(LinAlloc *linear_alloc, size_t size);



/// This function frees all the memory in the linear allocator
void lfree(LinAlloc *linear_alloc);
#endif // LIN_ALLOC_H
