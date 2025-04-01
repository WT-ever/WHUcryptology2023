#include <bits/stdc++.h>
using namespace std;
#define T 20
#define K 100
#undef RAND_MAX
#define RAND_MAX 37294702591
#define BIGINTN 80

inline char is_overflow(char a, char b) {
    if (a>0 && b>0 && a>CHAR_MAX-b) {
        return 1;
    }
    else if (a<0 && b<0 && a<CHAR_MIN-b) {
        return -1;
    }
    return 0;
}

string addBigInt(string a, string b) {
    char ia[BIGINTN], ib[BIGINTN], ic[BIGINTN];
    memset(ia, 0, BIGINTN);
    memset(ib, 0, BIGINTN);
    memset(ic, 0, BIGINTN);
    int lena, lenb;
    lena = a.size();
    lenb = b.size();
    int starta, startb;
    starta = a[0]=='-' ? 1 : 0;
    startb = b[0]=='-' ? 1 : 0;
    for (int i = starta; i < lena; i++) {
        ia[i] = starta ? -(a[lena-1-i]-'0') : (a[lena-1-i]-'0');
    }
    for (int i = startb; i < lenb; i++) {
        ib[i] = startb ? -(b[lenb-1-i]-'0') : (b[lenb-1-i]-'0');
    }
    for (int i = 0; i < max(lena, lenb); i++) {
        ic[i] = ia[i]+ib[i];
    }
    int i;
    for (i = 0; i<max(lena, lenb)+1 && (i<BIGINTN-1); i++) {
        if (ic[i]>=0) {
            ic[i+1] += ic[i]/10;
            ic[i] = ic[i]%10;
        }
        else {
            ic[i+1] += ic[i]/10;
            if (ic[i]%10==0) {
                ic[i]=0;
            }
            else {
                ic[i+1]--;
                ic[i] = CHAR_MAX+ic[i]%10;
            }
        }
    }
    i = ic[i-1]==0 ? i-1 : i;
    for (int j = 0; j < i; j++) {
        ic[j]+='0';
    }
    reverse(ic, ic+i);
    return string(ic);
}


long long binpow(long long m, long long e, long long n);

// 生成大素数
long long primeGen(long k) {
    bool isPrime = 0;
    srand(time(0));
    long long a, x, y;
    long long d, test_n;
    bool Pass;
    bool tmp_composite;
    while (!isPrime) {
        d = ((rand()%32767)+233)/2+1;
        int s = rand()%T+1;
        test_n = (long long)d*powl(2, s)+1;
        Pass = tmp_composite = 0;
        for (int i = 0; i < k; i++) {
            a = rand()%(test_n-2-2)+2;
            x = binpow(a, d, test_n);
            for (int j = 0; j < s; j++) {
                y = (x*x)%test_n;
                if (y==1 && x!=1 && x!=test_n-1) {
                    tmp_composite = 1;
                    break;
                }
                x = y;
            }
            if (y!=1) {
                tmp_composite = 1;
            }
            if (tmp_composite) {
                break;
            }
        }
        isPrime = 1;
    }
    return test_n;
}

// 模反复平方乘算法
long long binpow(long long m, long long e, long long n) {
    long long res = 1;
    while (e>0) {
        if (e&1) {
            res = (res*m)%n;
        }
        m = (m*m)%n;
        e >>= 1;
    }
    return res;
}

// 求逆元：返回值为gcd(a,b),x为a%b的逆元
long long gcd(long long a, long long b, long long &x, long long &y) {
    if (b==0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1; 
    long long d = gcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - y1*(a/b);
    return d;
}

// 密钥生成算法
void keysGen(long long &e, long long &d) {
    long long p, q, n, fn;
    p = primeGen(K);
    q = primeGen(K);
    n = p*q;
    if (n/p!=q || n<0) {
        puts("digit exceed limitation");
        return ;
    }
    fn = (p-1) * (q-1);
    srand(time(0));
    e = rand()%(fn-1)+1;
    long long y;
    d = y = 0;
    gcd(e, fn, d, y);
}