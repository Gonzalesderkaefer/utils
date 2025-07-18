// Header file
#include "jstring.h"

// Libraries
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>





typedef struct {
    String pub_str;
    const Alloc alloc;
    const DeAlloc dealloc;
} PrivString;


String *string_init(const char *init_str, const Alloc alloc, const DeAlloc dealloc) {
    // Sanity check
    if (alloc == NULL || dealloc == NULL) {
        return NULL;
    }


    // Allocate Empty string
    char *new_string = alloc(strlen(init_str) + 1);
    if (new_string == NULL) {
        return NULL;
    }
    snprintf(new_string, strlen(init_str) + 1, "%s", init_str);

    // Local String struct
    PrivString local_str = {
        .pub_str = (String) {
            .string = new_string,
            .length = 0,
        },
        .alloc = alloc,
        .dealloc = dealloc
    };

    // Allocate String on heap
    PrivString *heap_str = alloc(sizeof(PrivString));
    if (heap_str == NULL) {
        dealloc(new_string);
        return NULL;
    }
    memcpy(heap_str, &local_str, sizeof(PrivString));

    return &heap_str->pub_str;
}




void string_push(String *string, const char *str ) {
    if (string == NULL || str == NULL) {
        return;
    }
    size_t str_len = strlen(str);
    PrivString *priv_str = (void *)string - offsetof(PrivString, pub_str);
    Alloc alloc = priv_str->alloc;
    DeAlloc dealloc = priv_str->dealloc;

    // Allocate new combined string
    const size_t new_str_len = string->length + str_len + 1;
    // Allocate new bigger buffer
    char *new_str = alloc(new_str_len);
    if (new_str == NULL) {
        return;
    }
    // Concat into new buffer
    assert(string->string != NULL);
    snprintf(new_str, new_str_len, "%s%s", string->string, str);

    // Create new string
    String new_pub_str = {
        .string = new_str,
        .length = new_str_len - 1,
    };

    // Free old string
    dealloc((char *)string->string);


    // Copy new public string to priv string struct
    memcpy(&priv_str->pub_str, &new_pub_str, sizeof(String));
}



void string_free(String *string) {
    if (string == NULL) {
        return;
    }
    PrivString *priv_str = (void *)string - offsetof(PrivString, pub_str);
    DeAlloc dealloc = priv_str->dealloc;
    dealloc((void *)string->string);
    dealloc(priv_str);
}
