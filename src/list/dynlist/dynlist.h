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

static Allocator default_allocator = {
    .context = NULL,
    .alloc = __malloc_alloc,
    .dealloc = __malloc_free,
    .realloc = __malloc_realloc
};


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
