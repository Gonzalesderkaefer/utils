#ifndef __JAZZY_VECTOR_T_H
#define __JAZZY_VECTOR_T_H


#include <stddef.h>
#include <stdlib.h>
#include <string.h>



#define INIT_VEC_SIZE 16

typedef struct {
     size_t elem_size; // size of individual elements
     size_t cap; // Number of elements
     size_t len; // Number of elements
     void *storage;
} __vector_t;




static void *__vec_init(size_t elem_size) {
    // Allocate storage
    __vector_t *vect_memory = malloc(sizeof(__vector_t) + elem_size * INIT_VEC_SIZE);
    if (vect_memory == NULL) {
        return NULL;
    }

    // Get storage ptr
    void *storage_ptr = (char *)vect_memory + sizeof(__vector_t);

    // Assign values
    vect_memory->elem_size = elem_size;
    vect_memory->cap = INIT_VEC_SIZE;
    vect_memory->len = 0;
    vect_memory->storage = storage_ptr;



    // Will be implicitly casted
    return storage_ptr;
}


static void * __vec_resize(void *storage_ptr) {
    // Sanity check
    if (storage_ptr == NULL) {
        return NULL;
    }

    // Get vector
    __vector_t *vect = (__vector_t *)((char *)storage_ptr - sizeof(__vector_t));

    if (vect->len < vect->cap) {
        return storage_ptr;
    }

    // Allcoate new storage
    __vector_t *new_vect = malloc(sizeof(__vector_t) + vect->elem_size * vect->cap * 2);
    if (new_vect == NULL) {
        return NULL;
    }

    // Copy old values
    memcpy(new_vect, vect, sizeof(__vector_t) + vect->elem_size * vect->cap);

    // Get new storage ptr
    void *new_storage_ptr = (char *)new_vect + sizeof(__vector_t);

    // Assign values
    new_vect->elem_size = vect->elem_size;
    new_vect->cap = vect->cap * 2;
    new_vect->len = vect->len;
    new_vect->storage = new_storage_ptr;


    // Free old vector
    free(vect);

    return new_vect->storage;
}

static __vector_t *__vec_get_struct(void *storage_ptr)  {
    if (storage_ptr == NULL) {
        return NULL;
    }
    return (__vector_t *)((char *)storage_ptr - sizeof(__vector_t));
}



#define vec_push(ptr, elem) \
    if (ptr == NULL) { \
        ptr = __vec_init(sizeof(*ptr)); \
    } else { \
        ptr = __vec_resize(ptr); \
    } \
    if (ptr != NULL) { \
        ptr[(int)__vec_get_struct(ptr)->len] = elem; \
        __vec_get_struct(ptr)->len++; \
    } \


#define vec_free(ptr) \
    if (ptr != NULL) { \
        free(__vec_get_struct(ptr)); \
    } \

#define vec_len(ptr) \
    ((ptr == NULL) ? 0 : (__vec_get_struct(ptr)->len))



#endif // __VECTOR_T_H
