#include <bits/stdc++.h>
using namespace std;
#define T 20
#define K 100
#undef RAND_MAX
#define RAND_MAX 37294702591
#define BIGINTN 80
typedef long long BigInteger;

inline char is_overflow(char a, char b) {
    if (a>0 && b>0 && a>CHAR_MAX-b) {
        return 1;
    }
    else if (a<0 && b<0 && a<CHAR_MIN-b) {
        return -1;
    }
    return 0;
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
long long gcd(long long a, long long b, long long &x, long long &y, long long n) {
    if (b==0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1; 
    long long d = gcd(b, a%b, x1, y1, n);
    x = (y1)%n;
    y = (x1 - y1*(a/b))%n;
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
    gcd(e, fn, d, y, fn);
}

void enElGamal(BigInteger y, BigInteger m, BigInteger a, BigInteger k, BigInteger p, BigInteger &C1, BigInteger &C2) {
    BigInteger U;
    U = binpow(y, k, p);
    C1 = binpow(a, k, p);
    C2 = (U*m)%p;
}

void deElGamal(BigInteger C1, BigInteger C2, BigInteger p, BigInteger d, BigInteger &M) {
    BigInteger V, Vrev, tmp;
    V = binpow(C1, d, p);
    gcd(V, p, Vrev, tmp, p);
    M = (C2*Vrev)%p; 
}

void signElGamal(BigInteger M, BigInteger p, BigInteger k, BigInteger a, \
BigInteger d, BigInteger &r, BigInteger &s) {
    BigInteger invK, tmp;
    gcd(k, p, invK, tmp, p);
    r = binpow(a, k, p);
    s = ((M-d*r)*invK)%(p-1);
}

bool verifyElGamal(BigInteger a, BigInteger M, BigInteger y, \
BigInteger r, BigInteger s, BigInteger p) {
    return (binpow(a, M, p) == (binpow(y, r, p)*binpow(r, s, p))%p);
}