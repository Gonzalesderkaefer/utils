#include <stdio.h>

#if (__STDC_VERSION__ >= 202300L)
#define to_ptr(value) (typeof(value) [1]){value}
#else
#define to_ptr(type, value) (type [1]){value}
#endif


int print() {
    return 0;
}


int main(void) {
    printf("%p\n", to_ptr(3));
}
