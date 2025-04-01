#include <iostream>
using namespace std;
#define N 100
int main() {
    int R[N];
    string s;
    cin>>s;
    for (auto &c: s) {
        c = c-'0';
    }
    for (int t = 0; t < s.size(); t++) {
        R[t] = 0;
        for (int i = 0; i<s.size(); i++) {
            R[t] += (2*s[i]-1)*(2*s[i+t]-1);
        }
        printf("R[%d] = %lg\n", t, double(R[t])/15.0);
    }
}