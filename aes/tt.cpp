#include <bits/stdc++.h>
using namespace std;
int main()
{
    map<int, int> mm;
    // key i, value pingfangshengyu
    for (int i = 0; i < 13; i++) {
        mm[i] = (i*i)%13;
    }
    for (int i = 0; i < 13; i++) {
        int muti = pow(i, 3)+2*i+9;
        muti  = muti%13;
        printf("%d    %d", i, muti);
        int cnt = 0;
        for (int j = 0; j < 13; j++) {
            if (mm[j]==muti) {
                if (cnt++==0) {
                    printf("     Yes");
                }
                printf(" %d", j);
            }
        }
        if (cnt==0) {
            printf("     No");
        }
        else  {
            cnt=0;
        }
        putchar('\n');
    }
    return 0;
}