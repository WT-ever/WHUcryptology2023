#include <bits/stdc++.h>
#include "./elgamal.h"
using namespace std;

int main() {
    long long k, d, y, a, p, m, r, s;
    m = 14;
    d = 16; 
    y = 4;
    a = 10;
    p = 19;
    k = 5;
    signElGamal(m, p, k, a, d, r, s);
    printf("数字签名：(r, s) = (%lld, %lld)\n", r, 4);
    if (verifyElGamal(a, m, y, r, 4, p)) {
        puts("true");
    }
    return 0;
}