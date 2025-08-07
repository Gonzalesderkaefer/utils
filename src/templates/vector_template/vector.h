// Libraries
#include<stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/****************************** Alloc *****************************************/

#ifndef JAZZY_ALLOC_TYPES
#define JAZZY_ALLOC_TYPES

// Libraries
#include<stddef.h>
#include <stdlib.h>
#include <string.h>


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Params: - size_t: amount of bytes needed
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

/*************************** Vec Implementaion *****************************/


/// You have to add the ifdef gurads yourself because
/// the type of the vector is defined by you
///
/// Something like VECTOR_TYPE_H is recommendend where TYPE is the 
/// snake_type below but captilaized
#ifndef VEC_TYPE_H
#define VEC_TYPE_H
/*          ^^^^  Change this!! */

// Edit these
#define type int // The actual typename
#define snake_type int // Used for functions
#define pascal_type Int // Used for the vec typename





#define _Vec(t) Vec##t
#define __vec(t) vec_##t
#define VecOf(t) _Vec(t)
#define vecof(t) __vec(t)
#define Vec VecOf(pascal_type)
#define _vec vecof(snake_type)


#define concat2(a, b) a##b
#define comp2(a, b) concat2(a, b)



typedef struct Vec  {
    size_t cap;
    size_t len;
    AllocFunc alloc;
    FreeFunc dealloc;
    type *storage;
} Vec;



static Vec *comp2(_vec, _init)(AllocFunc alloc, FreeFunc dealloc) {
    // Both have to be defined or undefined
    if ((alloc == NULL) ^ (dealloc == NULL)) {
        return NULL;
    }

    // Allocate storage
    type *new_storage = alloc(sizeof(type) * 10);
    if (new_storage == NULL) {
        return NULL;
    }
    // Zero out
    memset(new_storage, 0, sizeof(type) * 10);

    // Allocate struct
    Vec *new_vect = alloc(sizeof(Vec));
    if (new_vect == NULL) {
        dealloc(new_storage);
        return NULL;
    }

    if (alloc == NULL && dealloc == NULL) {
        new_vect->alloc = malloc;
        new_vect->dealloc = free;
    } else {
        new_vect->alloc = alloc;
        new_vect->dealloc = dealloc;
    }
    new_vect->cap = 10;
    new_vect->len = 0;
    new_vect->storage = new_storage;


    return new_vect;
}


static void comp2(_vec, _push)(Vec *vec, const type value) {
    if (vec->len >= vec->cap) {
        // Allocate new storage
        type *new_storage = vec->alloc(sizeof(type) * vec->cap * 2);
        if (new_storage == NULL) {
            return;
        }
        // Zero out
        memset(new_storage, 0, sizeof(type) * vec->cap * 2);

        // Copy into new_storage 
        memcpy(new_storage, vec->storage, vec->len * sizeof(type));

        // Free old storage
        vec->dealloc(vec->storage);

        // Reassign new storage
        vec->storage = new_storage;

        // Update vec capacity
        vec->cap *= 2;
    }

    // Add new value 
    vec->storage[vec->len++] = (type)value;
}


static void comp2(_vec, _delete)(Vec *vec, size_t index) {
    // Sanity check
    if (vec == NULL || vec->len <= index) {
        return;
    }

    // Move everything one to the left
    for (uint64_t i = index; i < vec->len - 1; ++i) {
        vec->storage[i] = vec->storage[i + 1];
    }
}


static type *comp2(_vec, _storage)(const Vec *vec) {
    if (vec == NULL) {
        return NULL;
    } 
    return vec->storage;
}



static void comp2(_vec, _free)(Vec *vec) {
    // Store a pointer to the dealloc function
    FreeFunc dealloc = vec->dealloc;

    // Free storage
    dealloc(vec->storage);

    // Free struct itself
    dealloc(vec);
}


#undef type
#undef snake_type
#undef pascal_type
#undef _Vec
#undef __vec
#undef VecOf
#undef vecof
#undef Vec
#undef _vec




#endif // VEC_TYPE_H
