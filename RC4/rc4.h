#include <stdio.h>
#include <stdlib.h>

void RC4(unsigned char *plaintext, int plainSize, unsigned char *ciphertext,\
        unsigned char keyseed[256], int keySize) {
    // 初始化S盒
    unsigned char S[256];
    unsigned char T[256];
    unsigned char key;
    for (int i = 0; i < 256; i++) {
        S[i] = i;
        T[i] = keyseed[i%keySize];
    }

    // 用T产生S的初始置换
    int i, j;
    j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j+S[i]+T[i]) % 256;
        unsigned char tmp = S[i];
        S[i] = S[j];
        S[j] = tmp; // swap(S[i], S[j])
    } 

    // 利用PRGA来生成密钥流，并置换S
    i = j = 0;
    unsigned char t;
    for (int m = 0; m < plainSize; m++) {
        i = (i+1)%256;
        j = (j+S[i])%256;
        unsigned char tmp = S[i];
        S[i] = S[j];
        S[j] = tmp; // swap(S[i], S[j])

        t = (S[i]+S[j]) % 256;
        key = S[t]; // 产生密钥流
        ciphertext[m] = key^plaintext[m];
    }
}