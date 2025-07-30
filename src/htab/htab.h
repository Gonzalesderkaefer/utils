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

// TODO: Add documentation

typedef struct _Htab  Htab;

Htab *htab_init(AllocFunc alloc, FreeFunc free);

void htab_insert(Htab *htab, void *value, void *key, size_t keylen, bool allocated);

void *htab_lookup(Htab *htab, char *key, size_t keylen);

void htab_free(Htab *htab);

AllocFunc htab_alloc(Htab *htab);

FreeFunc htab_dealloc(Htab *htab);

#endif // HTAB_H
