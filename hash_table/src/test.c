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
    htab_put(ht, "zero", strlen("zero"), &zero);
    puts("");
    htab_put(ht, "one", strlen("one"), &one);
    puts("");
    htab_put(ht, "two", strlen("two"), &two);
    puts("");
    htab_put(ht, "three", strlen("three"), &three);
    puts("");
    htab_put(ht, "four", strlen("four"), &four);
    puts("");
    htab_put(ht, "five", strlen("five"), &five);
    puts("");
    htab_put(ht, "six", strlen("six"), &six);
    puts("");
    htab_put(ht, "seven", strlen("seven"), &seven);
    puts("");
    htab_put(ht, "eight", strlen("eight"), &eight);
    puts("");
    htab_put(ht, "nine", strlen("nine"), &nine);
    puts("");
    puts("");
    puts("");


    printf("%d\n", *(int *)htab_see(ht, "zero", strlen("zero")));
    printf("%d\n", *(int *)htab_see(ht, "one", strlen("one")));
    printf("%d\n", *(int *)htab_see(ht, "two", strlen("two")));
    printf("%d\n", *(int *)htab_see(ht, "three", strlen("three")));
    printf("%d\n", *(int *)htab_see(ht, "four", strlen("four")));
    printf("%d\n", *(int *)htab_see(ht, "five", strlen("five")));
    printf("%d\n", *(int *)htab_see(ht, "six", strlen("six")));
    printf("%d\n", *(int *)htab_see(ht, "seven", strlen("seven")));
    printf("%d\n", *(int *)htab_see(ht, "eight", strlen("eight")));
    printf("%d\n", *(int *)htab_see(ht, "nine", strlen("nine")));

    htab_free(ht);
}
