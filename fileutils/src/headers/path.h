/// Path
/// This struct is a String type that
/// can be used to work with filepaths.
/// It should make working with them
/// easier
typedef struct _Path  Path;


/// path_init()
/// This function initializes a `Path`
/// that initially holds the value of
/// it returns a pointer to a `Path`
/// on success.
/// On failure it returns NULL
Path *path_init(const char *initval);

/// path_append()
/// This function appends `appendix` to `path`
/// as a new filepath component
void path_append(Path *path, const char *appendix);

/// path_free()
/// This function has to be called after
/// `path` is not needed anymore
void path_free(Path *path);

/// path_str()
/// Returns the value of `path` as
/// a normal string
const char *path_str(Path *path);

/// path_base()
/// Returns the basename of the `path`
/// as a normal string
const char *path_base(Path *path);

/// path_parent()
/// Returns the parent of the `path`
/// as a normal string
const char *path_parent(Path *path);
