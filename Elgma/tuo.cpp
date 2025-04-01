#include "tuoyuan.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int mulexp[10] = {0};
    mulexp[0] = 6;
    mulexp[1] = mulexp[3] = 1;
    tuoyuan(11, mulexp); 
    return 0;
}