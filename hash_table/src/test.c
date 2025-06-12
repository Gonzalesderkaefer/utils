#include "hash_table.c"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>


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
    uint32_t zero = 0;
    uint32_t one = 1;
    uint32_t two = 2;
    uint32_t three = 3;
    uint32_t four = 4;
    uint32_t five = 5;
    uint32_t six = 6;
    uint32_t seven = 7;
    uint32_t eight = 8;
    uint32_t nine = 9;

    HashTable *ht = htab_init();
    htab_put(ht, (u_char *)"zero", strlen("zero"), &zero);
    htab_put(ht, (u_char *)"one", strlen("one"), &one);
    htab_put(ht, (u_char *)"two", strlen("two"), &two);
    htab_put(ht, (u_char *)"three", strlen("three"), &three);
    htab_put(ht, (u_char *)"four", strlen("four"), &four);
    htab_put(ht, (u_char *)"five", strlen("five"), &five);
    htab_put(ht, (u_char *)"six", strlen("six"), &six);
    htab_put(ht, (u_char *)"seven", strlen("seven"), &seven);
    htab_put(ht, (u_char *)"eight", strlen("eight"), &eight);
    htab_put(ht, (u_char *)"nine", strlen("nine"), &nine);

    printf("%d\n", *((int *)htab_see(ht, (u_char *)"one", strlen("one"))));
    printf("%d\n", *((int *)htab_see(ht, (u_char *)"two", strlen("two"))));
    //printf("%d\n", *((int *)htab_see(ht, (u_char *)"three", strlen("three"))));

    htab_free(ht);
}
