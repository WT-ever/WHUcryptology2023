#include <stdio.h>
#define N 1024/5
int main() {
    FILE *fptr;
    fptr = fopen("in.txt", "w");
    for (int i = 0; i < N; i++) {
        fprintf(fptr, "66554433221100");
    }
}