#define _XOPEN_SOURCE 500 // This is needed for strtok_r

// Header file
#include "fileutils.h"



// Libraries
#include <dirent.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "../path/path.h"
#include "../../list/list.h"



/// This function creates a the directory speicified
/// by `path` and also creates its parent directories
/// on error this function returns -1. Cause of this
/// is either memory allocation failure, in which case
/// errno will be set according to malloc() or a failure
/// caused by mkdir(2) in which case errno will be
/// set according to mkdir(2)

int mkdir_p(const char *path, const mode_t mode) {
    // Copy path to buffer on stack (for strtok_r)
    size_t pathlen = strlen(path) + 1; // Determine length for buffer
    char pathbuf[pathlen]; // Create buffer
    snprintf(pathbuf, pathlen, "%s", path); // Copy into buffer

    // Dynamically growing buffer
    char *current_buffer = (char *)calloc(2,sizeof(char));
    if (!current_buffer) { // Error checking
        return -1;
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
            return -1;
        }

        // Copy current buffer and token into new buffer
        snprintf(new_buffer, newlen, "%s%s/", current_buffer, token);

        // We don't need the old buffer
        free(current_buffer);


        // Reassign old variable to new buffer
        current_buffer = new_buffer;

        // Create the dir ignore when it does not
        if (access(current_buffer, F_OK) == -1) // Make sure dir does not exist
            if (mkdir(current_buffer, mode) != 0){ // Error handling
                free(current_buffer); // We have to free this
                return -1;
            } 
    }

    free(current_buffer);
    return 0;
}

/// This function copies
int copy_file(const char *src, const char *dest) {
    // Open source file for reading
    FILE *srcfp = fopen(src, "r");
    if (!srcfp) // Error checking
        return -1;

    // Determine file size
    fseek(srcfp, 0, SEEK_END); // Move to end
    const size_t src_size = ftell(srcfp); // Get file-position
    fseek(srcfp, 0, SEEK_SET); // Move to beginning

    // Allocate buffer for data from source file
    char *src_buffer = (char *)calloc(src_size + 1, sizeof(char));
    if (!src_buffer) { // Error checking
        fclose(srcfp); // We have to close this file
        return -1;
    }

    // Read from file into buffer
    fread(src_buffer, sizeof(char), src_size, srcfp);
    if (ferror(srcfp) == 0) { // Error checking for fread see man fread and man ferror
        return -1;
    }

    // Open dest file for writing
    FILE *destfp = fopen(dest, "w");

    if (!destfp) { // Error checking
        fclose(srcfp); // We have to close this ...
        free(src_buffer); // We have to free this ...
        return -1;
    }

    // Write to destination file
    fwrite(src_buffer, sizeof(char), src_size, destfp);
    if (ferror(destfp) == 0) { // Error checking for fread see man fread and man ferror
        return -1;
    }

    // Cleanup
    fclose(srcfp);
    free(src_buffer);
    fclose(destfp);

    return 0;
}

/// This function copies the directory specified at `srcdir`
/// to `destdir` where destdir will not be the new parent dir
/// but will contain everyting that `srcdir` contained, if `destdir`
/// does not exist it will be created. On success this function returns 0
/// on error it returns -1. Cause for error is failure to create
/// `destdir` in which case errno will be set according to mkdir() or
/// or if source does not exist in which case only -1 is returned
int copy_r(const char *srcdir, const char *destdir) {
    if (access(srcdir, F_OK) == -1) { // If source does not exist do nothing
        return -1;
    }


    if (access(destdir, F_OK) == -1) // If dest does not exist, create the directory
        if (mkdir_p(destdir, 0755) == -1)
            return -1;


    // Open source directory
    DIR *source_dir = opendir(srcdir);
    if (!source_dir) // Error checking
        return -1;

    struct dirent *dircontent; // This is one directory entry
    // Iterate through directory
    while((dircontent = readdir(source_dir))) {
        // Skip this dir (".") and parent dir ("..")
        if (!strcmp(".", dircontent->d_name) || !strcmp("..", dircontent->d_name))
            continue;


        // Initialize source path
        Path *src = path_init(srcdir);
        if (!src) { // Error checking
            continue; // Just move on to the next entry (for now..)
        }


        // Initialize destination path
        Path *dest = path_init(destdir);
        if (!dest) {
            path_free(src); // We have to free this ...
            continue; // Move on to next entry (for now..)
        }

        // Append basename of direntry to dest
        path_append(dest, dircontent->d_name);

        // Append basename of direntry to src
        path_append(src, dircontent->d_name);

        // Check for access, if no access skip
        if (access(path_str(src), R_OK) == -1) continue;

        // Metadata for source
        struct stat srcstat;
        lstat(path_str(src), &srcstat); // Actually get Metadata


        if (S_ISDIR(srcstat.st_mode)) {
            mkdir(path_str(dest), 0755); // Create dir in dest since
            printf("creating %s\n", path_str(dest));
            if (copy_r(path_str(src), path_str(dest)) == -1) // Error checking for recursive call
                continue; // Skip if there was an error
        } else {
            printf("%s to %s\n", path_str(src), path_str(dest));
            copy_file(path_str(src), path_str(dest));
        }


        // Cleanup
        path_free(dest);
        path_free(src);
    }
    // Cleanup
    closedir(source_dir);
    return 0; 
}
