#include "./path.c"


int main(void) {
    assert(getenv("HOME") != NULL);
    Path *configsrc = path_init(getenv("HOME"));
    path_append(configsrc, "Jazzian/dotfiles/");
    path_append(configsrc, "config/");
    printf("%s\n", configsrc->buf);
    path_free(configsrc);
}
