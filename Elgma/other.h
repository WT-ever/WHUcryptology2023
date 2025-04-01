#include "./elgamal.h"

void sign(BigInteger a, BigInteger k, BigInteger d, BigInteger p, BigInteger &A, \
BigInteger B, BigInteger C, BigInteger &r) {
    r = binpow(a, k, p);
    A = (B + C*d)%(p-1);
}

bool verify(BigInteger r, BigInteger A, BigInteger a, BigInteger C,\
 BigInteger y, BigInteger B, BigInteger p) {
    return binpow(r, A, p) == binpow(a, C, p)*binpow(y, B, p);
}