#ifndef region_ALLOC_H
#define region_ALLOC_H
// Libraries
#include <stddef.h>

/// Block is 16MiB in size by default if not definded otherwise
#ifndef blocksize
#define blocksize 16777216
#endif

/// These macros should make using the allocators easier


/// This initializes the macro interface for the region allocators
#define glob_region_init() \
    RegAlloc glob_mem = {0}; \
    RegAlloc *cur_mem = &glob_mem; \
    RegAlloc *last_mem = NULL


/// This initializes the macro interface for the region allocators
#define region_init() \
    static RegAlloc glob_mem = {0}; \
    static RegAlloc *cur_mem = &glob_mem; \
    RegAlloc *last_mem = NULL


/// create a new allocator to use
#define region_switch(new_allocator) \
    RegAlloc new_allocator = {0}; \
    last_mem = cur_mem; \
    cur_mem = &new_allocator

/// Free used allocator and assign current allocator
/// back
#define region_free(new_allocator) \
    lfree(&new_allocator);\
    assert(last_mem != NULL); \
    cur_mem = last_mem




/// This struct represents a region of memeory
typedef struct _Block Block;

/// This struct represents a handle to a
/// region allocator and is passed around as a pointer
/// You have to init it with:
/// ```
/// RegAlloc glob_mem = {0};
/// ```
typedef struct _RegAlloc {
    Block *first;
    Block *current;
} RegAlloc;

/// This function alloctaes a chunk of memory of size
/// in the region allocator.
void *lalloc(RegAlloc *region_alloc, size_t size);



/// This function frees all the memory in the region allocator
void lfree(RegAlloc *region_alloc);
#endif // region_ALLOC_H
