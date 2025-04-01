#include <stdio.h>
#define PRINT_BINARY(num)            \
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) { \
        printf("%d", (num >> i) & 1); \
    }                                \
    printf("\n")

int main() {
    unsigned int arr[8];
    for (int i = 0; i < 8; i++) {
        scanf("%u", &arr[i]);
    }
    for (int i =0; i < 8; i++) {
        PRINT_BINARY(arr[i]);
        putchar('\n');
    }
    return 0;
}