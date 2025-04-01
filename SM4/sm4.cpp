#include "./sm4.h"
#include <stdio.h>
#include <string.h>
#include <ctime>
int MODE = 0;

// 将大端字节数组转换为unsigned long
inline void BEtoUL(unsigned long &n, unsigned char* b, int i) {
    n = (b[i]<<24) | (b[i+1]<<16) | (b[i+2]<<8) | (b[i+3]);
}

// 将unsigned long转换为大端字节数组
inline void ULtoBE(unsigned long &n, unsigned char *b, int i) {
    b[i] = n>>24;
    b[i+1] = n>>16;
    b[i+2] = n>>8;
    b[i+3] = n;
}

// 循环右移
inline unsigned long rotl(unsigned long x, int n) {
    return ((x&0xFFFFFFFF) << n) | (x>>(32-n));
}

inline void swap(unsigned long &a, unsigned long &b) {
    unsigned long t;
    t = a;
    a = b;
    b = t;
}

// S盒 
static unsigned char Sbox[16][16] = 
{
    {0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05},
    {0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99},
    {0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62},
    {0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6},
    {0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8},
    {0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35},
    {0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87},
    {0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e},
    {0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1},
    {0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3},
    {0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f},
    {0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51},
    {0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8},
    {0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0},
    {0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84},
    {0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48}
};

// 密钥拓展算法中的常数FK
static unsigned long FK[4] = {0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc};

// 密钥拓展算法中的固定参数CK
static unsigned long CK[32] =
{
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};


// S盒变换, 进行字节的映射
static unsigned char sm4Sbox(unsigned char onebyte) {
    return Sbox[onebyte/16][onebyte%16];
}

/*
 * 线性变换部件L和合成变换T的组合
 * 输入为一个32位字
 * 输出为一个32位字
 */
static unsigned long sm4Lt(unsigned long ka) {
    unsigned long bb = 0;
    unsigned long c = 0;
    unsigned char a[4];
	unsigned char b[4];
    ULtoBE(ka,a,0);
    b[0] = sm4Sbox(a[0]);
    b[1] = sm4Sbox(a[1]);
    b[2] = sm4Sbox(a[2]);
    b[3] = sm4Sbox(a[3]);
	BEtoUL(bb,b,0);
    c = bb ^ (rotl(bb, 2))^(rotl(bb, 10))^(rotl(bb, 18))^(rotl(bb, 24));
    return c;
}

/* 
 * 轮函数F
 * 输入为四个32位字，和一个轮密钥rk
 * 输出为一个字
 */
static unsigned long sm4F(unsigned long x0, unsigned long x1, 
    unsigned long x2, unsigned long x3, unsigned long rk) {
    return (x0 ^ sm4Lt(x1^x2^x3^rk));
}


/* 
 * 计算轮密钥
 * 输入为一个32位字
 * 输出为一个32位字
 */
static unsigned long sm4CalciRK(unsigned long ka) {
    unsigned long bb = 0;
    unsigned long rk = 0;
    unsigned char a[4];
    unsigned char b[4];
    ULtoBE(ka,a,0);
    b[0] = sm4Sbox(a[0]);
    b[1] = sm4Sbox(a[1]);
    b[2] = sm4Sbox(a[2]);
    b[3] = sm4Sbox(a[3]);
	BEtoUL(bb,b,0);
    rk = bb ^ (rotl(bb, 13)) ^ (rotl(bb, 23));
    return rk;
}

/*
 * 轮密钥拓展函数
 * subkeys[32]为轮密钥存储地
 * key[16]为初始密钥
 * mode选择加密或解密
 */
void sm4_setkey( unsigned long subkeys[32], unsigned char key[16], int mode) {
    MODE = mode;
    unsigned long MK[4];
    unsigned long k[36];
    unsigned long i = 0;

    BEtoUL( MK[0], key, 0 );
    BEtoUL( MK[1], key, 4 );
    BEtoUL( MK[2], key, 8 );
    BEtoUL( MK[3], key, 12 );
    k[0] = MK[0] ^ FK[0];
    k[1] = MK[1] ^ FK[1];
    k[2] = MK[2] ^ FK[2];
    k[3] = MK[3] ^ FK[3];
    for(; i<32; i++) {
        k[i+4] = k[i] ^ (sm4CalciRK(k[i+1]^k[i+2]^k[i+3]^CK[i]));
        subkeys[i] = k[i+4];
	}
    if (mode==SM4_DECRYPT) {
        for( i = 0; i < 16; i ++ ) {
            swap(subkeys[i], subkeys[31-i] );
        }
    }
}

// 一轮SM4
static void sm4_one_round( unsigned long sk[32], unsigned char input[16], unsigned char output[16] ) {
    unsigned long i = 0;
    unsigned long ulbuf[36];

    memset(ulbuf, 0, sizeof(ulbuf));
    BEtoUL( ulbuf[0], input, 0 );
    BEtoUL( ulbuf[1], input, 4 );
    BEtoUL( ulbuf[2], input, 8 );
    BEtoUL( ulbuf[3], input, 12 );
    while(i<32) {
        ulbuf[i+4] = sm4F(ulbuf[i], ulbuf[i+1], ulbuf[i+2], ulbuf[i+3], sk[i]);
	    i++;
    }
	ULtoBE(ulbuf[35],output,0);
	ULtoBE(ulbuf[34],output,4);
	ULtoBE(ulbuf[33],output,8);
	ULtoBE(ulbuf[32],output,12);
}

/* 
 * ECB模式SM4加解密函数
 * subkeys为32个轮密钥
 * mode 选择加密或者解密
 * length 为输入数据长度
 * 4字节输入数据
 * 4字节输出数据
 */
void sm4_crypt_ecb( unsigned long *subkeys, int mode,
				   int length, unsigned char *input, unsigned char *output) {
    while( length > 0 ) {
        sm4_one_round( subkeys, input, output );
        input  += 16;
        output += 16;
        length -= 16;
    }
}

/* 
 * CBC模式SM4加解密函数
 * length 为应得明文的长度,而不是密文的长度，这在短块解密时至关重要！
 * iv为初始化向量
 */
void sm4_crypt_cbc( unsigned long *subkeys, int mode, int length,
        unsigned char original_iv[16], unsigned char *input, unsigned char *output ) {
    int i;
    unsigned char temp[16];
    unsigned char iv[16];
    memcpy(iv, original_iv, 16);
    unsigned char formal_block[16];

    if(mode==SM4_ENCRYPT) {
        while (length>0) {
            if (length<16) {
                memcpy(formal_block, iv+length, 16-length);
                memcpy(formal_block+16-length, input, length);
            } // 进行短块的处理
            else {
                memcpy(formal_block, input, 16);
            }
            for( i = 0; i < 16; i++ )
                output[i] = (unsigned char)( formal_block[i] ^ iv[i] );

            sm4_one_round( subkeys, output, output );
            memcpy( iv, output, 16 );

            input  += 16;
            output += 16;
            length -= 16;
        }
    }
    else {
        while (length>0) {
            memcpy( temp, input, 16 );
            sm4_one_round( subkeys, input, output );
            for( i = 0; i < 16; i++ ) {
                output[i] = (unsigned char)( output[i] ^ iv[i] );
            }
            memcpy( iv, temp, 16 );
            if (length<16) {
                unsigned char short_block[16];
                memcpy(short_block, output+16-length, length);
                memcpy(output, short_block, length);
            }
            input  += 16;
            output += 16;
            length -= 16;
        }
    }
}

/*
 * 计算加解密所用时间
 * key为初始128位密钥
 * 后面的参数参照sm4_crypt_cbc
 * 返回时间单位为us
 */
double cal_time( unsigned char *key, int mode, int length,
        unsigned char original_iv[16], unsigned char *input, 
        unsigned char *output ) {
    // 获取开始时间
    clock_t start = clock();
    
    // 调用要计时的函数
    unsigned long sk[32];
    sm4_setkey(sk, key, mode);
    for (int i = 0; i < 10000; i++) {
        sm4_crypt_cbc(sk, mode, length, original_iv, input, output);
    }
    
    // 获取结束时间
    clock_t end = clock();
    
    double time_taken = (double) (1000*1000/10000) * (end - start)/CLOCKS_PER_SEC;
    
    return time_taken;
}