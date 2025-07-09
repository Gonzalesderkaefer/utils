// The Header file
#include "path.h"

// Libraries
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>




typedef struct _Path {
    const char *buf; // Pointer to char buffer, with NUL terminator
    const char *basename; // the last component of the path
    const char *parent; // Everything except the last component of the path
    size_t buflen; // current buffer length includes NUL termintator
    void *(*alloc) (size_t bytes); // allocator function is usually malloc()
    void (*dealloc) (void *); // deallocator function is usually free()
} Path;


Path *path_init_alloc(const char *initval, void *(*alloc) (size_t bytes), void (*dealloc) (void *)) {
    // Check if initval contains '//'
    if (strstr(initval, "//")) {
        fprintf(stderr, "initval is malformed");
        return NULL;
    }

    char *new_buf; // Pointer to new buffer
    size_t new_len; // length of new buffer
    if (strlen(initval) == 0) { // special case if initial value is empty 
        new_len = 1;
        new_buf = (char *)alloc(2); // Allocate buffer with length 2 for '/' and NUL

        if (new_buf == NULL) // Error check
            return NULL;

        snprintf(new_buf, 2,"/"); // Write empty string to buffer


    } else if (*initval == '/') { // Check if initval starts with '/'
        new_len = strlen(initval) + 1; // Store length
        new_buf = (char *)alloc(strlen(initval) + 1); // Allocate buffer

        if (new_buf == NULL) // Error check
            return NULL;

        snprintf(new_buf, new_len, "%s", initval); // write initval to buffer
    } else { // Initval starts without '/' so have to append it
        new_len = strlen(initval) + 2; // Store length
        new_buf = (char *)alloc(strlen(initval) + 2); // Allocate buffer

        if (new_buf == NULL) // Error check
            return NULL;

        snprintf(new_buf, new_len, "/%s", initval); // write initval to buffer
                                                  // prepended with a '/'
    }

    char *parbuf = (char *)alloc(new_len); // Allocate partent buffer
    if (parbuf == NULL) { // Error check
        dealloc((void *)new_buf); // We have to free this
        return NULL;
    }
    snprintf(parbuf, new_len, "%s", new_buf); // Copy into parentbuf


    if (parbuf[strlen(parbuf) - 1] == '/') { // check if path ends with '/'
        parbuf[strlen(parbuf) - 1] = '\0'; // If so delete it
    }


    char *basename = strrchr(parbuf, '/'); // Find where base name starts
    // If there's a basename
    if (basename != NULL) {
        *(basename++) = '\0'; // cut off the parent there
    }

    Path *newpath = (Path *)alloc(sizeof(Path)); // Allocate Path struct
    if (!newpath) {
        dealloc(new_buf); // We have to free these
        dealloc(parbuf);
        return NULL;
    }

    // Allocated fields
    newpath->basename = basename;
    newpath->parent = parbuf;
    newpath->buf = new_buf;
    newpath->buflen = new_len;
    newpath->alloc = alloc;
    newpath->dealloc = dealloc;

    return newpath;
}







Path *path_init(const char *initval) {
    // Check if initval contains '//'
    if (strstr(initval, "//")) {
        fprintf(stderr, "initval is malformed");
        return NULL;
    }

    char *new_buf; // Pointer to new buffer
    size_t new_len; // length of new buffer
    if (strlen(initval) == 0) { // special case if initial value is empty 
        new_len = 1;
        new_buf = (char *)malloc(2); // Allocate buffer with length 2 for '/' and NUL

        if (!new_buf) // Error check
            return NULL;

        snprintf(new_buf, 2,"/"); // Write empty string to buffer


    } else if (*initval == '/') { // Check if initval starts with '/'
        new_len = strlen(initval) + 1; // Store length
        new_buf = (char *)malloc(strlen(initval) + 1); // Allocate buffer

        if (!new_buf) // Error check
            return NULL;

        snprintf(new_buf, new_len, "%s", initval); // write initval to buffer
    } else { // Initval starts without '/' so have to append it
        new_len = strlen(initval) + 2; // Store length
        new_buf = (char *)malloc(strlen(initval) + 2); // Allocate buffer

        if (!new_buf) // Error check
            return NULL;

        snprintf(new_buf, new_len, "/%s", initval); // write initval to buffer
                                                  // prepended with a '/'
    }

    char *parbuf = (char *)malloc(new_len); // Allocate partent buffer
    if (!parbuf) { // Error check
        free((void *)new_buf); // We have to free this
        return NULL;
    }
    snprintf(parbuf, new_len, "%s", new_buf); // Copy into parentbuf


    if (parbuf[strlen(parbuf) - 1] == '/') { // check if path ends with '/'
        parbuf[strlen(parbuf) - 1] = '\0'; // If so delete it
    }


    char *basename = strrchr(parbuf, '/'); // Find where base name starts
    *(basename++) = '\0'; // cut off the parent there

    Path *newpath = (Path *)malloc(sizeof(Path)); // Allocate Path struct
    if (!newpath) {
        free(new_buf); // We have to free these
        free(parbuf);
        return NULL;
    }

    // Allocated fields
    newpath->basename = basename;
    newpath->parent = parbuf;
    newpath->buf = new_buf;
    newpath->buflen = new_len;
    newpath->alloc = malloc;
    newpath->dealloc = free;

    return newpath;
}

void path_append_to_base(Path *path, const char *appendix) {
    // Check if path is complete
    if (!path || !path->alloc || !path->dealloc || 
            !path->basename || !path->buf || !path->buflen || !path->parent)
        return;

    // No slash at beginning of appendix allowed
    if (*appendix == '/')
        return;

    // Get the last char of the buffer
    char lastchar = path->buf[path->buflen - 2];

    // length of the new buffer
    size_t newlen;
    if (lastchar != '/') { // If there's a '/' we don't need the extra character
        newlen = path->buflen + strlen(appendix);
    } else {
        newlen = path->buflen + strlen(appendix) + 1;
    }

    // Allocate new main buffer
    char *newbuf = (char *)path->alloc(newlen);
    if (newbuf == NULL) {
        return;
    }

    // Allocate new parent buffer
    char *parbuf = (char *)path->alloc(newlen);
    if (parbuf == NULL) {
        path->dealloc(newbuf); // We have to free this...
        return;
    }

    if (lastchar == '/') { // If the buffer ended with '/' don't add the '/' in between
        // Write into new main buffer
        snprintf(newbuf, newlen, "%s%s", path->buf, appendix);
    } else {
        // Write into new main buffer
        snprintf(newbuf, newlen, "%s%s/", path->buf, appendix);
    }

    // Write into new parent buffer
    snprintf(parbuf, newlen, "%s%s", path->buf, appendix);

    // Find last slash
    char *basename = strrchr(parbuf, '/');
    if (basename != NULL) { // For now return if no last '/' was found
        *(basename++) = '\0';
    }

    // Free old buffers
    path->dealloc((void *)path->buf);
    path->dealloc((void *)path->parent);


    // Reassign
    path->buf = newbuf;
    path->parent = parbuf;
    path->buflen = newlen;
    path->basename = basename;


}

void path_append(Path *path, const char *appendix) {
    // Check if path is complete
    if (!path || !path->alloc || !path->dealloc || 
            !path->basename || !path->buf || !path->buflen || !path->parent)
        return;

    // No slash at beginning of appendix allowed
    if (*appendix == '/')
        return;

    // Get the last char of the buffer
    char lastchar = path->buf[path->buflen - 2];


    // length of the new buffer
    size_t newlen;
    if (lastchar == '/') { // If there's a '/' we don't need the extra character
        newlen = path->buflen + strlen(appendix);
    } else {
        newlen = path->buflen + strlen(appendix) + 1;
    }

    // Allocate new main buffer
    char *newbuf = (char *)path->alloc(newlen);
    if (newbuf == NULL)
        return;

    // Allocate new parent buffer
    char *parbuf = (char *)path->alloc(newlen);
    if (parbuf == NULL) {
        path->dealloc(newbuf); // We have to free this...
        return;
    }


    if (lastchar == '/') { // If the buffer ended with '/' don't add the '/' in between
        // Write into new main buffer
        snprintf(newbuf, newlen, "%s%s", path->buf, appendix);

        // Write into new parent buffer
        snprintf(parbuf, newlen, "%s%s", path->buf, appendix);
    } else {
        // Write into new main buffer
        snprintf(newbuf, newlen, "%s/%s", path->buf, appendix);

        // Write into new parent buffer
        snprintf(parbuf, newlen, "%s/%s", path->buf, appendix);
    }


    // Remove slash at the end for parent
    if (parbuf[newlen - 2] == '/')
        parbuf[newlen - 2] = '\0';

    // Find last slash
    char *basename = strrchr(parbuf, '/');
    if (basename != NULL) { // For now return if no last '/' was found
        *(basename++) = '\0';
    }

    // Free old buffers
    path->dealloc((void *)path->buf);
    path->dealloc((void *)path->parent);


    // Reassign
    path->buf = newbuf;
    path->parent = parbuf;
    path->buflen = newlen;
    path->basename = basename;
}


void path_free(Path *path) {
    if (!path) // Sanity check
        return;

    path->dealloc((void *)path->buf); // free main buffer
    path->dealloc((void *)path->parent); // free parent buffer
    path->dealloc(path); // free the buffer
}


const char *path_str(Path *path) {
    return path->buf;
}

const char *path_base(Path *path) {
    return path->basename;
}

const char *path_parent(Path *path) {
    return path->parent;
}
