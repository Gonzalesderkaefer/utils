// Header file
#include "list.h"

typedef struct _List {
    size_t capacity;
    size_t length;
    size_t type_size;
    char *storage;
} List;



List *list_init(size_t type_size) {
    // Allocate struct
    List *new_list = (List *)malloc(sizeof(List));
    if (new_list == NULL) { // Error checking
        return NULL;
    }

    // Allocate storage
    void *new_storage = calloc(10, type_size);
    if (new_storage == NULL) { // Error checking
        free(new_list); // We have to free this ...
        return NULL;
    }

    // Assign values
    new_list->type_size = type_size;
    new_list->storage = new_storage;
    new_list->capacity = 10;
    new_list->length = 0;


    return new_list;
}


void *list_storage(List *list) {
    return list->storage;
}


size_t list_len(List *list) {
    return list->length;
}


void list_push(List *list, void *value) {
    // use value as a byte stirng
    char *value_bytes = value;

    if (list->length >= list->capacity) {
        // Allcoate new storage as bytes
        char *new_storage = calloc(list->capacity * 2, list->type_size);
        if (new_storage == NULL) // Error checking
            return;

        // Copy old bytes into new storage
        for (size_t i = 0; i < list->length * list->type_size; ++i) {
            new_storage[i] = list->storage[i];
        }

        // Free old bytes
        free(list->storage);

        // Reassign
        list->storage = new_storage;

        // Update capacity
        list->capacity *= 2;
    }
        for (size_t i = 0; i < list->type_size; ++i) {
            list->storage[list->length * list->type_size + i] = value_bytes[i];
        }
        list->length++;
}





void list_free(List *list) {
    // Free storage
    free(list->storage);

    // Free struct
    free(list);
}
