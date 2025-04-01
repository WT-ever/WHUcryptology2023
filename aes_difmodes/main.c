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
void low_s(unsigned char des[16], unsigned char src[16], int s);
void mvl_s(unsigned char des[16], unsigned char src[16], int s);

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

void OFB() {
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
        puts("请输入所取位数s");
        scanf("%d", &s);

        keyExpansion(K, k);
        unsigned char opt[16], opt_new[16];
        memcpy(opt, ztext, 16);
        for (int i = 0; i < group_n; i++) {
            AES(opt, opt_new, k, 10);
            low_s(opt_new, opt_new, s); //取低s位
            XOR(ciphertext[i], opt_new, plaintext[i]);
            mvl_s(opt, opt, s);
            for (int i =  0; i < s/8+1; i++) {
                opt[i] = opt_new[i] | opt[i];
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
        puts("请输入所取位数s");
        scanf("%d", &s);

        keyExpansion(K, k);
        unsigned char opt[16], opt_new[16];
        memcpy(opt, ztext, 16);
        for (int i = 0; i < group_n; i++) {
            AES(opt, opt_new, k, 10);
            low_s(opt_new, opt_new, s); //取低s位
            XOR(plaintext[i], opt_new, ciphertext[i]);
            mvl_s(opt, opt, s); // 左移s位
            for (int i =  0; i < s/8+1; i++) {
                opt[i] = opt_new[i] | opt[i]; //拼接
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

void CFB() {

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
        puts("请输入所取位数s");
        scanf("%d", &s);

        keyExpansion(K, k);
        unsigned char opt[16], opt_new[16];
        memcpy(opt, ztext, 16);
        for (int i = 0; i < group_n; i++) {
            AES(opt, opt_new, k, 10);
            low_s(opt_new, opt_new, s); //取低s位
            XOR(ciphertext[i], opt_new, plaintext[i]);
            low_s(opt_new, ciphertext[i], s);
            mvl_s(opt, opt, s);
            for (int i =  0; i < s/8+1; i++) {
                opt[i] = opt_new[i] | opt[i];
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
        puts("请输入所取位数s");
        scanf("%d", &s);

        keyExpansion(K, k);
        unsigned char opt[16], opt_new[16];
        memcpy(opt, ztext, 16);
        for (int i = 0; i < group_n; i++) {
            AES(opt, opt_new, k, 10);
            low_s(opt_new, opt_new, s); //取低s位
            XOR(plaintext[i], opt_new, ciphertext[i]);
            low_s(opt_new, ciphertext[i], s);
            mvl_s(opt, opt, s);
            for (int i =  0; i < s/8+1; i++) {
                opt[i] = opt_new[i] | opt[i];
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

void CTR() {}

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

        puts("请选择分组密码工作模式 (ECB:0、CBC:1、OFB:2、CFB:3、CTR:4)");
        scanf("%d", &mode);
        switch (mode) {
            case ECBmode:
                ECB();
                break;
            case CBCmode:
                CBC();
                break;
            case OFBmode:
                OFB();
                break;
            case CFBmode:
                CFB();
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

void low_s(unsigned char des[16], unsigned char src[16], int s) {
    int save_bytes, save_bits;
    save_bytes = s/8;
    save_bits = s%8;
    memcpy(des, src, 16);
    for (int i = 0; i<15-save_bytes; i++) {
        memset(&des[i], 0, 1);
    }
    des[15-save_bytes] = (des[15-save_bytes]<<(8-save_bits))>>(8-save_bits);
}

void mvl_s(unsigned char des[16], unsigned char src[16], int s) {
    if (s==0) {
        memcpy(des, src, 16);
        return ;
    }
    unsigned char shift_bytes, shift_bits, overflow;
    shift_bytes = s/8;
    shift_bits = s%8;
    overflow = 0;
    for (int i = 0; i < 16; i++) {
        if (i+shift_bytes<16) {
            des[i] = src[i+shift_bytes];
        }
        else {
            des[i] = 0;
        }
    } // 先位移字节

    unsigned char next_overflow;
    for (int i = 15; i >=0; i--) {
        next_overflow = des[i]>>(8-shift_bits);
        des[i] = (des[i]<<shift_bits) | overflow;
        overflow = next_overflow;
    }
}