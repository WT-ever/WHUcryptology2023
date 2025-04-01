#include <stdio.h>
#include <bits/stdc++.h>
#include "rsa.h"
using namespace std;

BigInteger RSA(BigInteger key, BigInteger m, BigInteger n) {
    return binpow(m, key, n);
}