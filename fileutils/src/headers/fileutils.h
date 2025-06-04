#ifndef FILEUTILS_H
#define FILEUTILS_H


/// This function essenially creates the entire `path` that is passed
/// it works like mkdir -p
void mkdir_p(const char *path);


/// This function copies the file at `src` to `dest`
void copy_file(const char *src, const char *dest);


/// This function copies `srcdir` to `dest` where
/// `dest` will become the new parent directory
void copy_r(const char *srcdir, const char dest);







#endif //FILEUTILS_H
