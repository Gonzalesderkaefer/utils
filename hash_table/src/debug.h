// Libraries
#include<stdlib.h>
#include<stdio.h>



void *debug_malloc(size_t size) {
    void *to_return = malloc(size);
    if (to_return == NULL) {
        printf("Failed to Allocate!\n"); 
    } else {
        printf("Allocate: %p\n", to_return); 
    }
    return to_return;
}

void *debug_calloc(size_t nmemb, size_t size) {
    void *to_return = calloc(nmemb, size);
    if (to_return == NULL) {
        printf("Failed to Allocate!\n"); 
    } else {
        printf("Allocate: %p\n", to_return); 
    }
    return to_return;
}

void debug_free(void *ptr) {
    printf("Free: %p\n", ptr); 
    free(ptr);
}


