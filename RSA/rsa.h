#include <bits/stdc++.h>
#include "../BigInteger/BigInteger.h"
using namespace std;
#define T 20
#define K 100
#undef RAND_MAX
#define RAND_MAX 37294702591
#define BIGINTN 80


// 生成大素数
BigInteger primeGen(long k) {
    bool isPrime = 0;
    srand(time(0));
    BigInteger a, x, y;
    BigInteger d, test_n;
    bool Pass;
    bool tmp_composite;
    while (!isPrime) {
        d = ((rand()%32767)+233)/2+1;
        int s = rand()%T+1;
        test_n = d*powl(2, s)+1;
        Pass = tmp_composite = 0;
        for (int i = 0; i < k; i++) {
            BigInteger tmp = rand();
            a = tmp%(test_n-2-2)+2;
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
BigInteger binpow(BigInteger m, BigInteger e, BigInteger n) {
    BigInteger res = 1;
    while (e>0) {
        if ((e%2)==1) {
            res = (res*m)%n;
        }
        m = (m*m)%n;
        e = e/2;
    }
    return res;
}

// 求逆元：返回值为gcd(a,b),x为a%b的逆元
BigInteger gcd(BigInteger a, BigInteger b, BigInteger &x, BigInteger &y) {
    if (b==0) {
        x = 1;
        y = 0;
        return a;
    }
    BigInteger x1, y1; 
    BigInteger d = gcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - y1*(a/b);
    return d;
}

// 密钥生成算法
void keysGen(BigInteger &e, BigInteger &d) {
    BigInteger p, q, n, fn;
    p = primeGen(K);
    q = primeGen(K);
    n = p*q;
    if (n/p!=q || n<0) {
        puts("digit exceed limitation");
        return ;
    }
    fn = (p-1) * (q-1);
    srand(time(0));
    BigInteger tmp;
    tmp = rand();
    e = tmp%(fn-1)+1;
    BigInteger y;
    d = 0;
    y = 0;
    gcd(e, fn, d, y);
}