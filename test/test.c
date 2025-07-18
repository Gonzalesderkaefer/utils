#include <stdio.h>

int main(void) {
    int three = 3;
    typeof(three) four = 4;
    printf("%d\n", four);
}
