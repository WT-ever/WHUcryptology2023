#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "./des.h"
void HexToChar8(unsigned char m[17], ElemType Block[8]) {
    for (int i = 0; i < 8; i++) {
        Block[i] = m[i*2]*16 + m[i*2+1]*1;
    }
}

void Char8ToHex(ElemType Block[8], unsigned char m[17]) {
    for (int i = 0; i < 8; i++) {
        m[i*2+1] = Block[i]%16;
        m[i*2] = (Block[i]/16) % 16;
    }
}

void StringToHex(unsigned char str[17], unsigned char hex[16]) {
    for (int i = 0; i < 16; i++) {
        if (isdigit(str[i])) {
            hex[i] = str[i]-'0';
        }
        else if (isupper(str[i])) {
            hex[i] = str[i]-'A'+10;
        }
        else if (islower(str[i])) {
            hex[i] = str[i]-'a'+10;
        }
    }
}

int main() {
    puts("输入1开始加密，输入2开始解密，输入3退出");
    char c;
    ElemType cipherBlock[8], plainBlock[8];
    ElemType keyBlock[8], key[64], subKeys[16][48];
    unsigned char cipher_s[17], plain_s[17], key_s[17];
    unsigned char cipher_hex[16], plain_hex[16], key_hex[16];

    while ((c=getchar())!='3') {
        memset(cipherBlock, 0, 8);
        memset(plainBlock, 0, 8);
        memset(key, 0, 64);
        memset(subKeys, 0, 16*48);
        if (c=='1') {
            puts("请输入64bits十六进制明文");
            scanf("%s", plain_s);
            puts("请输入64bits十六进制密钥");
            scanf("%s", key_s); //输入明文和密钥

            StringToHex(plain_s, plain_hex);
            HexToChar8(plain_hex, plainBlock);
            StringToHex(key_s, key_hex);
            HexToChar8(key_hex, keyBlock);
            Char8ToBit64(keyBlock, key);   //转换格式

            DES_MakeSubKeys(key, subKeys);
            DES_EncryptBlock(plainBlock, subKeys, cipherBlock); //加密


            Char8ToHex(cipherBlock, cipher_hex);
            printf("密文为：");
            for (int i = 0; i < 16 ;i++) {
                printf("%x", cipher_hex[i]);
            }
        }
        else if (c=='2') {
            puts("请输入64bits十六进制密文");
            scanf("%s", cipher_s);
            puts("请输入64bits十六进制密钥");
            scanf("%s", key_s); //输入明文和密钥

            StringToHex(cipher_s, cipher_hex);
            HexToChar8(cipher_hex, cipherBlock);
            StringToHex(key_s, key_hex);
            HexToChar8(key_hex, keyBlock);
            Char8ToBit64(keyBlock, key);   //转换格式

            DES_MakeSubKeys(key, subKeys);
            DES_DecryptBlock(cipherBlock, subKeys, plainBlock); //解密


            Char8ToHex(plainBlock, plain_hex);
            printf("明文为：");
            for (int i = 0; i < 16; i++) {
                printf("%x", plain_hex[i]);
            }
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