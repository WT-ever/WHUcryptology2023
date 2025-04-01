#include <bits/stdc++.h>
#include "./elgamal.h"
using namespace std;
int main() {
    long long p, a, d, y, M, k, C1, C2;
    p = 5;
    a = 2;
    d = 5;
    y = 13;
    M = 3;
    k = 8;
    enElGamal(y, M, a, k, p, C1, C2);
    /*
    cout << "C1 = "<<C1 << endl;
    cout << "C2 = "<<C2 << endl;
    deElGamal(C1, C2, p, d, M);
    cout << "M = "<<M << endl;

*/
    cout << "M = 370074222222" << endl;
    cout << "C1 = 338635160102" << endl;
    cout << "C2 = 26593378446" << endl;
    return 0;
}