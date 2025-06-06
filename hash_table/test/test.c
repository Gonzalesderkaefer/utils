
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


typedef struct _Person {
    uint8_t age;
    char name[];
} Person;

Person max = {
    .age = 31,
    .name = {'M', 'a', 'x'},
};


void iterate(char *bytes, size_t size) {
    for (uint32_t u = 0; u < size; ++u) {
        printf("%d\n", bytes[u]);
    }
}

int main(void) {

    uint32_t max_size = sizeof(Person);
    printf("%d\n", max_size);
}
