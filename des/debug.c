#include <stdio.h>
#include <string.h>
#include <ctype.h>
typedef unsigned char ElemType;
#define DEBUGMAKEKEYS
#undef DEBUGMAKEKEYS
#define DEBUGROL
#undef DEBUGROL
#define DEBUGXOR
#undef DEBUGXOR
#define DEBUGSBOX
#undef DEBUGSBOX
#define DEBUG64TO8
#undef DEBUG64TO8
/*置换选择1*/
int PC_1[56] = { 56,48,40,32,24,16,8,
0,57,49,41,33,25,17,
9,1,58,50,42,34,26,
18,10,2,59,51,43,35,
62,54,46,38,30,22,14,
6,61,53,45,37,29,21,
13,5,60,52,44,36,28,
20,12,4,27,19,11,3 };
/*置换选择2*/
int PC_2[48] = { 13,16,10,23,0,4,2,27,
14,5,20,9,22,18,11,3,
25,7,15,6,26,19,12,1,
40,51,30,36,46,54,29,39,
50,44,32,47,43,48,38,55,
33,52,45,41,49,35,28,31 };
/*对左移次数的规定*/
int MOVE_TIMES[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

/*生成子密钥*/
void DES_PC1_Transform(ElemType key[64], ElemType temp[56]) {
    for (int i = 0; i < 56; i++) {
        temp[i] = key[PC_1[i]];
    }
}

void DES_PC2_Transform(ElemType temp[56], ElemType i_subKeys[48]) {
    for (int i = 0; i < 48; i++) {
        i_subKeys[i] = temp[PC_2[i]];
    }
}

//左移：左右两部分分别独立左移
void DES_ROL(ElemType temp[56], int shift_bits) {
    #ifdef DEBUGROL
    puts("Before rol:");
    puts("Left part:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 7; j++) {
            printf("%d", temp[i*7+j]); 
        }
        putchar('\n');
    }
    puts("Right part:");
    for (int i = 0; i<4; i++) {
        for (int j = 0;j < 7; j++) {
            printf("%d", temp[i*7+j+28]);
        }
        putchar('\n');
    }
    #endif
    ElemType tmp_temp[56];
    for (int i = 0; i < 56; i++) {
        tmp_temp[i] = temp[i];
    }
    for (int i = 0; i < 28; i++) {
        temp[(i-shift_bits+28)%28] = tmp_temp[i];
        temp[(i-shift_bits+28)%28+28] = tmp_temp[i+28];
    }
    #ifdef DEBUGROL
    puts("After rol:");
    puts("Left part:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 7; j++) {
            printf("%d", temp[i*7+j]); 
        }
        putchar('\n');
    }
    puts("Right part:");
    for (int i = 0; i<4; i++) {
        for (int j = 0;j < 7; j++) {
            printf("%d", temp[i*7+j+28]);
        }
        putchar('\n');
    }
    #endif
 
}

int DES_MakeSubKeys (ElemType key[64], ElemType subKeys[16] [48]) {
    ElemType temp[56];
    #ifdef DEBUGMAKEKEYS
    puts("key:");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", key[i*8+j]);
        }
        putchar('\n');
    }
    #endif
    int cnt;
    DES_PC1_Transform(key, temp);  /*PC1置换*/
    #ifdef DEBUGMAKEKEYS
    puts("temp:");
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", temp[i*8+j]);
        }
        putchar('\n');
    }
    #endif
    for (cnt = 0; cnt < 16; cnt++) {      /*16轮迭代，产生16个子密钥*/
        DES_ROL(temp, MOVE_TIMES[cnt] ) ;    /*循环左移*/
        DES_PC2_Transform(temp, subKeys[cnt]) ;    /*PC2置换，产生子密钥*/
    }
    return 0;
}

/*IP置换*/
/*初始置换表IP*/
int IP_Table[64] = { 57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7,
56,48,40,32,24,16,8,0,
58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6 };

int DES_IP_Transform(ElemType data [64]) {
    int cnt;
    ElemType temp [64];
    for (cnt = 0; cnt < 64; cnt++) {
        temp [cnt] = data[IP_Table[cnt]] ;
    }
    memcpy(data, temp, 64);
    return 0;
}

/*IP逆置换*/
/*逆初始置换表IP^-1*/
int IP_1_Table[64] = { 39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41,9,49,17,57,25,
32,0,40,8,48,16,56,24 };

int DES_IP_1_Transform(ElemType data [64]) {
    int cnt;
    ElemType temp [64];
    for (cnt = 0; cnt < 64; cnt++) {
        temp [cnt] = data[IP_1_Table [cnt] ] ;
    }
    memcpy(data, temp, 64);
    return 0;
}

/*选择运算E*/
/*扩充置换表E*/
int E_Table[48] = { 31, 0, 1, 2, 3, 4,
3, 4, 5, 6, 7, 8,
7, 8,9,10,11,12,
11,12,13,14,15,16,
15,16,17,18,19,20,
19,20,21,22,23,24,
23,24,25,26,27,28,
27,28,29,30,31, 0 };
int DES_E_Transform(ElemType data[48]) {
    int cnt;
    ElemType temp[48];
    for (cnt = 0; cnt < 48; cnt++) {
        temp[cnt] = data[E_Table[cnt]];
    }
    memcpy(data, temp, 48);
    return 0;
}

/*S盒*/
int S[8][4][16] =
/*S1*/
{ { { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 } },
/*S2*/
{ { 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 } },
/*S3*/
{ { 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 } },
/*S4*/
{ { 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 } },
/*S5*/
{ { 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 } },
/*S6*/
{ { 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 } },
/*S7*/
{ { 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 } },
/*S8*/
{ { 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 } } };

void DES_SBOX(ElemType copyRight[48]) {
    ElemType temp[32];
    #ifdef DEBUGSBOX
    puts("48 bits before SBOX");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%d", copyRight[i*6+j]);
        }
        putchar('\n');
    }
    #endif
    for (int i = 0; i < 8; i++) {
        int index = i*6;
        int row, col;
        row = copyRight[index]*2+copyRight[index+5];
        col = copyRight[index+1]*8 + copyRight[index+2]*4 +\
        copyRight[index+3]*2 + copyRight[index+4]; //计算行号和列号

        int num_d = S[i][row][col];
        for (int j = 0; j < 4; j++) {
            temp[i*4+3-j] = num_d%2;
            num_d = num_d/2;
        } // 将十进制转化为二进制
    }
    memcpy(copyRight, temp, 32);
    #ifdef DEBUGSBOX
    puts("32 bits after SBOX");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d", copyRight[i*4+j]);
        }
        putchar('\n');
    }
    #endif
}

/*P置换*/
/*置换函数P*/
int P_Table[32] = { 15,6,19,20,28,11,27,16,
0,14,22,25,4,17,30,9,
1,7,23,13,31,26,2,8,
18,12,29,5,21,10,3,24 };
int DES_P_Transform(ElemType data [32]) {
    int cnt;
    ElemType temp [32];
    for (cnt = 0; cnt < 32; cnt++) {
        temp [cnt] = data[P_Table [cnt] ] ;
    }
    memcpy(data, temp, 32);
    return 0;
}

//异或操作
void DES_XOR(ElemType copyRight[48], ElemType i_subKeys[48], int size) {
    #ifdef DEBUGXOR
    puts("copyRight:");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", copyRight[i*8+j]);
        }
        putchar('\n');
    }
    puts("i_subKeys:");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", i_subKeys[i*8+j]);
        }
        putchar('\n');
    }
    #endif
    for (int i = 0; i < size; i++) {
        copyRight[i] = copyRight[i] ^ i_subKeys[i];
    }
    #ifdef DEBUGXOR
    puts("copyRight after XOR:");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", copyRight[i*8+j]);
        }
        putchar('\n');
    }
    #endif
}

void Char8ToBit64(ElemType Block[8], ElemType Bits[64]) {
    for (int i = 0; i < 8; i++) {
        int now_char8 = Block[i];
        for (int j = 0; j < 8; j++) {
            Bits[i*8+7-j] = now_char8%2;
            now_char8 = now_char8/2;
        }
    }
}

void Bit64ToChar8(ElemType Bits[64], ElemType Block[8]) {
    #ifdef DEBUG64TO8
    puts("Bits:");
    for (int i = 0; i<8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", Bits[i*8+j]);
        }
        putchar('\n');
    }
    #endif
    for (int i = 0; i < 8; i++) {
        int power_two = 1;
        for (int j = 0; j < 8; j++) {
            Block[i] += Bits[i*8+7-j]*power_two;
            power_two *= 2;
        }
    }
    #ifdef DEBUG64TO8
    puts("Block:");
    for (int i = 0; i < 8; i++) {
        printf("%d ", Block[i]);
    }
    #endif
} 

void DES_Swap(ElemType leftBits[32], ElemType rightBits[32]) {
    for (int i = 0; i < 32; i++) {
        ElemType tmp = leftBits[i];
        leftBits[i] = rightBits[i];
        rightBits[i] = tmp;
    }
}

/*加密单个分组*/
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]) {
    ElemType plainBits [64];
    ElemType copyRight [48];
    int cnt;

    Char8ToBit64(plainBlock, plainBits);
    /*初始置换(IP置换)*/
    DES_IP_Transform(plainBits);

    /*16轮迭代*/
    for (cnt = 0; cnt < 16; cnt++) {
        memcpy(copyRight, plainBits + 32, 32);

        /*将右半部分进行扩展置换,从32位扩展到48位*/
        DES_E_Transform(copyRight);

        /*将右半部分与子密钥进行异或操作*/
        DES_XOR(copyRight, subKeys[cnt], 48);

        /*异或结果进入S盒,输出32位结果*/
        DES_SBOX(copyRight);

        /*P置换*/
        DES_P_Transform(copyRight);

        /*将明文左半部分与右半部分进行异或*/
        DES_XOR(plainBits, copyRight, 32);
        if (cnt != 15) {
            /*最终完成左右部的交换*/
            DES_Swap(plainBits, plainBits + 32);
        }
    }
    /*逆初始置换(IP^1置换)*/
    DES_IP_1_Transform(plainBits);
    Bit64ToChar8(plainBits, cipherBlock);
    return 0;
}

void inverse_subKeys(ElemType subKeys[16][48]) {
    ElemType tmp_subKeys[48];
    for (int i = 0; i < 8; i++) {
        memcpy(tmp_subKeys, subKeys[i], 48);
        memcpy(subKeys[i], subKeys[15-i], 48);
        memcpy(subKeys[15-i], tmp_subKeys, 48);
    }
}

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
            puts("请输入十六进制明文");
            scanf("%s", plain_s);
            puts("请输入十六进制密钥");
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
            puts("请输入十六进制密文");
            scanf("%s", cipher_s);
            puts("请输入十六进制密钥");
            scanf("%s", key_s); //输入明文和密钥

            StringToHex(cipher_s, cipher_hex);
            HexToChar8(cipher_hex, cipherBlock);
            StringToHex(key_s, key_hex);
            HexToChar8(key_hex, keyBlock);
            Char8ToBit64(keyBlock, key);   //转换格式

            DES_MakeSubKeys(key, subKeys);
            inverse_subKeys(subKeys);
            DES_EncryptBlock(cipherBlock, subKeys, plainBlock); //解密


            Char8ToHex(plainBlock, plain_hex);
            printf("明文为：");
            for (int i = 0; i < 16; i++) {
                printf("%x", plain_hex[i]);
            }
        }
        else if (c=='\n') {}
        else {
            puts("invalid command");
        }
        puts("\n----------------------------------");
    }
    
    return 0;
}