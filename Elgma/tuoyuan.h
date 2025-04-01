#include <bits/stdc++.h>
using namespace std;

void tuoyuan(int p, int mulexp[10]) {
    long long fx;
    map<int, vector<int>> square_left;
    for (int i =  0; i < p; i++) {
        square_left[(i*i)%p].push_back(i);  
    }
    for (int i = 0; i < p; i++) {
        fx = 0;
        for (int j = 0; j < 10; j++) {
            if (mulexp[j]) {
                fx += pow(i, j)*mulexp[j];
                fx = fx%p;
            }
        } // 计算多项式的值
        if (!square_left[fx].empty()) {
            for (auto t: square_left[fx]) {
                printf("(%d, %d) ", i, t);
            }
            putchar('\n');
        }
    }
}