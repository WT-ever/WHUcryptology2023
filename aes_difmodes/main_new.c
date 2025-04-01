#include "./modes.h"
#include "../aes/aes.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define N 10

char c;
int group_n;
char cipher_s[N][33], plain_s[N][33], key_s[33], z_s[33];
unsigned char K[16], k[11][16];
unsigned char plaintext[N][16], ciphertext[N][16], ztext[16];
enum Mode mode = ECBmode;
int s;

unsigned char htod(unsigned char hex);
unsigned char dtoh(unsigned char digit);
void StringToBytes(char str[33], unsigned char bytes[16]);
void BytesToString(unsigned char bytes[16],char str[33]);
void XOR(unsigned char des[16], unsigned char src1[16], unsigned char src2[16]);

void ECB() {
    if (c=='1') {
        puts("请输入明文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制明文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", plain_s[i]);
            StringToBytes(plain_s[i], plaintext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入明文和密钥
        StringToBytes(key_s, K);

        keyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            AES(plaintext[i], ciphertext[i], k, 10);
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(ciphertext[i], cipher_s[i]);
            printf("第%d密文为：", i+1);
            printf("%s ,\n", cipher_s[i]);
        }
    }
    else if (c=='2') {
        puts("请输入密文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制密文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", cipher_s[i]);
            StringToBytes(cipher_s[i], ciphertext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入密文和密钥
        StringToBytes(key_s, K);

        InvKeyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            InvAES(ciphertext[i], plaintext[i], k, 10);
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(plaintext[i], plain_s[i]);
            printf("第%d组明文为：", i+1);
            printf("%s ,\n", plain_s[i]);
        }
    }
    else {
        puts("invalid command");
    }
}

void CBC() {
    if (c=='1') {
        puts("请输入明文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制明文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", plain_s[i]);
            StringToBytes(plain_s[i], plaintext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入明文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        keyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            XOR(ztext, ztext, plaintext[i]);
            AES(ztext, ciphertext[i], k, 10);
            memcpy(ztext, ciphertext[i], 16);
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(ciphertext[i], cipher_s[i]);
            printf("第%d密文为：", i+1);
            printf("%s ,\n", cipher_s[i]);
        }
    }
    else if (c=='2') {
        puts("请输入密文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制密文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", cipher_s[i]);
            StringToBytes(cipher_s[i], ciphertext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入密文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        InvKeyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            InvAES(ciphertext[i], plaintext[i], k, 10);
            if (i==0) {
                XOR(plaintext[i], plaintext[i], ztext);
            }
            else {
                XOR(plaintext[i], plaintext[i], ciphertext[i-1]);
            }
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(plaintext[i], plain_s[i]);
            printf("第%d组明文为：", i+1);
            printf("%s ,\n", plain_s[i]);
        }
    }
    else {
        puts("invalid command");
    }
}

void OFB_128() {
    if (c=='1') {
        puts("请输入明文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制明文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", plain_s[i]);
            StringToBytes(plain_s[i], plaintext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入明文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        keyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            AES(ztext, ciphertext[i], k, 10);
            memcpy(ztext, ciphertext[i], 16);
            XOR(ciphertext[i], plaintext[i], ciphertext[i]);
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(ciphertext[i], cipher_s[i]);
            printf("第%d密文为：", i+1);
            printf("%s ,\n", cipher_s[i]);
        }
    }
    else if (c=='2') {
        puts("请输入密文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制密文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", cipher_s[i]);
            StringToBytes(cipher_s[i], ciphertext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入密文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        keyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            AES(ztext, plaintext[i], k, 10);
            memcpy(ztext, plaintext[i], 16);
            XOR(plaintext[i], plaintext[i], ciphertext[i]);
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(plaintext[i], plain_s[i]);
            printf("第%d组明文为：", i+1);
            printf("%s ,\n", plain_s[i]);
        }
    }
    else {
        puts("invalid command");
    } 
}

void OFB_8() {}

void CFB_128() {}

void CFB_8() {

    if (c=='1') {
        puts("请输入明文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制明文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", plain_s[i]);
            StringToBytes(plain_s[i], plaintext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入明文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        keyExpansion(K, k);
        unsigned char opt[16], opt_new[16];
        memcpy(opt, ztext, 16);
        for (int i = 0; i < group_n; i++) {
            for (int j = 0; j < 16; j++) {
                AES(opt, opt_new, k, 10);
                ciphertext[i][j] = opt_new[15] ^ plaintext[i][j];
                for (int k = 0; k < 15; k++) {
                    opt[k] = opt[k+1];
                }
                opt[15] = ciphertext[i][j];
            }
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(ciphertext[i], cipher_s[i]);
            printf("第%d密文为：", i+1);
            printf("%s ,\n", cipher_s[i]);
        }
    }
    else if (c=='2') {
        puts("请输入密文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制密文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", cipher_s[i]);
            StringToBytes(cipher_s[i], ciphertext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入密文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        keyExpansion(K, k);
        unsigned char opt[16], opt_new[16];
        memcpy(opt, ztext, 16);
        for (int i = 0; i < group_n; i++) {
            for (int j = 0; j<16; j++) {
                AES(opt, opt_new, k, 10);
                plaintext[i][j] = ciphertext[i][j] ^ opt_new[15];
                for (int k = 0; k < 15; k++) {
                    opt[k] = opt[k+1];
                }
                opt[15] = ciphertext[i][j];
            }
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(plaintext[i], plain_s[i]);
            printf("第%d组明文为：", i+1);
            printf("%s ,\n", plain_s[i]);
        }
    }
    else {
        puts("invalid command");
    } 
}

void CTR() {
    if (c=='1') {
        puts("请输入明文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制明文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", plain_s[i]);
            StringToBytes(plain_s[i], plaintext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入明文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        unsigned char ti[16], oi[16];
        memcpy(ti, ztext, 16);
        ti[15] = 0;
        keyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            AES(ti, oi, k, 10);
            XOR(ciphertext[i], plaintext[i], oi);
            ti[15]++;
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(ciphertext[i], cipher_s[i]);
            printf("第%d密文为：", i+1);
            printf("%s ,\n", cipher_s[i]);
        }
    }
    else if (c=='2') {
        puts("请输入密文组数");
        scanf("%d", &group_n);
        puts("请输入128bits十六进制密文组");
        for (int i = 0; i < group_n; i++) {
            scanf("%32s", cipher_s[i]);
            StringToBytes(cipher_s[i], ciphertext[i]);
        }

        puts("请输入128bits十六进制密钥");
        scanf("%32s", key_s); //输入密文和密钥
        StringToBytes(key_s, K);
        puts("请输入128bits十六进制初始化向量");
        scanf("%32s", z_s);
        StringToBytes(z_s, ztext);

        unsigned char ti[16], oi[16];
        memcpy(ti, ztext, 16);
        ti[15] = 0;
        keyExpansion(K, k);
        for (int i = 0; i < group_n; i++) {
            AES(ti, oi, k, 10);
            XOR(plaintext[i], ciphertext[i], oi);
            ti[15]++;
        }

        for (int i = 0; i < group_n; i++) {
            BytesToString(plaintext[i], plain_s[i]);
            printf("第%d组明文为：", i+1);
            printf("%s ,\n", plain_s[i]);
        }
    }
    else {
        puts("invalid command");
    }
}

int main() {
    puts("输入1开始加密，输入2开始解密，输入3退出");
    while ((c=getchar())!='3') {
        if (c=='\n') {continue;}
        else if (c!='1' && c!='2') {
            puts("invalid input");
            puts("输入1开始加密，输入2开始解密，输入3退出");
            continue;
        }
        memset(K, 0, 16);
        memset(k, 0, 11*16);
        memset(plaintext, 0, 16);
        memset(ciphertext, 0, 16);

        puts("请选择分组密码工作模式 (ECB:0、CBC:1、OFB_128:2、OFB_8:3、CFB_128:4、CFB_8:5、CTR:6)");
        scanf("%d", &mode);
        switch (mode) {
            case ECBmode:
                ECB();
                break;
            case CBCmode:
                CBC();
                break;
            case OFB_128mode:
                OFB_128();
                break;
            case OFB_8mode:
                OFB_8();
                break;
            case CFB_128mode:
                CFB_128();
            case CFB_8mode:
                CFB_8();
                break;
            case CTRmode:
                CTR();
                break;
            default:
                puts("invalid input");
                break;
        }
        puts("\n----------------------------------");
        puts("输入1开始加密，输入2开始解密，输入3退出");
    }
    
    return 0;
}


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

void XOR(unsigned char des[16], unsigned char src1[16], unsigned char src2[16]) {
    for (int i = 0; i < 16; i++) {
        des[i] = src1[i] ^ src2[i];
    }
}