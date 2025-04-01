#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "./aes.h"
#define DEBUGK
#define DEBUGKK

unsigned char htod(unsigned char hex) {
    if (isdigit(hex)) {
        return hex-'0';
    }
    else if (isupper(hex)) {
        return hex -'A'+10;
    }
    else if (islower(hex)) {
        return hex -'a'+10;
    }
    return 0;
}

unsigned char dtoh(unsigned char digit) {
    if (digit>=0 && digit<10) {
        return '0'+digit;
    }
    else if (digit>=10 && digit <16) {
        return 'A'+digit-10;
    }
    puts("error in dtoh");
    return 0;
}

void StringToBytes(char str[33], unsigned char bytes[16]) {
    for (int i = 0; i < 16; ++i) {
        bytes[i] = 16*htod(str[2*i]) + htod(str[2*i+1]);
    }
}

void BytesToString(unsigned char bytes[16],char str[33]) {
    for (int i = 0; i < 16; ++i) {
        str[2*i] = dtoh((bytes[i]>>4) % 16);
        str[2*i+1] = dtoh(bytes[i]%16);
    }
    str[32] = '\0';
}


int main() {
    puts("输入1开始加密，输入2开始解密，输入3退出");
    char c;
    char cipher_s[33], plain_s[33], key_s[33];
    unsigned char K[16], k[11][16];
    unsigned char plaintext[16], ciphertext[16];

    while ((c=getchar())!='3') {
        memset(K, 0, 16);
        memset(k, 0, 11*16);
        memset(plaintext, 0, 16);
        memset(ciphertext, 0, 16);
        if (c=='1') {
            puts("请输入128bits十六进制明文");
            scanf("%s", plain_s);
            puts("请输入128bits十六进制密钥");
            scanf("%s", key_s); //输入明文和密钥

            StringToBytes(plain_s, plaintext);
            StringToBytes(key_s, K);
            keyExpansion(K, k);
    #ifdef DEBUGKK
     for (int i = 0; i < 16; i++) {
        printf("%d ", K[i]);
     }
     putchar('\n');
     #endif

            AES(plaintext, ciphertext, k, 10);

     #ifdef DEBUGK
     for (int i = 0; i < 11; i++) {
        printf("%d: ", i);
        for (int j = 0; j < 16; j++) {
            printf("%d ", k[i][j]);
        }
        putchar('\n');
     }
     #endif
            BytesToString(ciphertext, cipher_s);
            printf("密文为：");
            puts(cipher_s);
        }
        else if (c=='2') {
            puts("请输入128bits十六进制密文");
            scanf("%s", cipher_s);
            puts("请输入128bits十六进制密钥");
            scanf("%s", key_s); //输入明文和密钥

            StringToBytes(cipher_s, ciphertext);
            StringToBytes(key_s, K);
            InvKeyExpansion(K, k);
            InvAES(ciphertext, plaintext, k, 10);

            BytesToString(plaintext, plain_s);
            printf("密文为：");
            puts(plain_s);
        }
        else if (c=='\n') {continue;}
        else {
            puts("invalid command");
        }
        puts("\n----------------------------------");
        puts("输入1开始加密，输入2开始解密，输入3退出");
    }
    
    return 0;
}