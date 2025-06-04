#define _XOPEN_SOURCE 500 // This is needed for strtok_r
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

// Header file
#include "headers/fileutils.h"



// Libraries
#include <dirent.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "headers/path.h"




/// This function essenially creates the entire `path` that is passed
/// it works like mkdir -p
void mkdir_p(const char *path) {
    // Copy path to buffer on stack (for strtok_r)
    size_t pathlen = strlen(path) + 1; // Determine length for buffer
    char pathbuf[pathlen]; // Create buffer
    snprintf(pathbuf, pathlen, "%s", path); // Copy into buffer

    // Dynamically growing buffer
    char *current_buffer = (char *)calloc(2,sizeof(char));
    if (!current_buffer) { // Error checking
        return;
    }
    current_buffer[0] = '/'; // Initialize with '/'

    char *str, *token; // for (strtok_r)
    str = pathbuf; // Str has to point to the buffer
    while ((token = strtok_r(str, "/", &str)) != NULL) {
        // Size of currentbuffer, token and slash in between
        const size_t newlen = strlen(current_buffer) + strlen(token) + 2;

        // Allocate new buffer with new length
        char *new_buffer = (char *)calloc(newlen, sizeof(char));
        if (!new_buffer) { // Error checking
            free(current_buffer); // We have to free this ...
            return;
        }

        // Copy current buffer and token into new buffer
        snprintf(new_buffer, newlen, "%s%s/", current_buffer, token);

        // We don't need the old buffer
        free(current_buffer);


        // Reassign old variable to new buffer
        current_buffer = new_buffer;

        // Create the dir ignore when it does not
        mkdir(current_buffer, 0755);
    }

    free(current_buffer);
}

/// This function copies the file at `src` to `dest`
void copy_file(const char *src, const char *dest) {
    // Open source file for reading
    FILE *srcfp = fopen(src, "r");
    if (!srcfp) // Error checking
        return;

    // Determine file size
    fseek(srcfp, 0, SEEK_END); // Move to end
    const size_t src_size = ftell(srcfp); // Get file-position
    fseek(srcfp, 0, SEEK_SET); // Move to beginning

    // Allocate buffer for data from source file
    char *src_buffer = (char *)calloc(src_size + 1, sizeof(char));
    if (!src_buffer) { // Error checking
        fclose(srcfp); // We have to close this file
        return;
    }

    // Read from file into buffer
    fread(src_buffer, sizeof(char), src_size, srcfp);

    // Open dest file for writing
    FILE *destfp = fopen(dest, "w");

    if (!destfp) { // Error checking
        fclose(srcfp); // We have to close this ...
        free(src_buffer); // We have to free this ...
        return;
    }

    // Write to destination file
    fwrite(src_buffer, sizeof(char), src_size, destfp);

    // Cleanup
    fclose(srcfp);
    free(src_buffer);
    fclose(destfp);
}

/// This function copies `srcdir` to `dest` where
/// `dest` will become the new parent directory
void copy_r(const char *srcdir, const char dest) {
    // Open source directory
    DIR *source_dir = opendir(srcdir);
    if (!source_dir) // Error checking
        return;

    struct dirent *dircontent; // This is one directory entry
    // Iterate through directory
    while((dircontent = readdir(source_dir))) {


    }


    // Cleanup
    closedir(source_dir);
}



#endif // FILE_UTILS_H
