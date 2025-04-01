#ifndef SM4HEAD
#define SM4HEAD

#define SM4_ENCRYPT 1
#define SM4_DECRYPT 0

extern int MODE; // 全局变量，表示加密1或解密0

/* 
 * 轮密钥拓展函数
 * subkeys存储拓展后的32个轮密钥
 * key[16]为初始的128bit密钥，存储单位为一个字节 
 */
void sm4_setkey(unsigned long subkeys[32], unsigned char key[16], int mode);

/* 
 * ECB模式SM4加解密函数
 * subkeys为32个轮密钥
 * mode 选择加密或者解密
 * length 为输入的明文，或应该得到的明文的长度
 * 输入数据（以字节为基本单位，不限长度）
 * 输出数据（以字节为基本单位，不限长度）
 */
void sm4_crypt_ecb( unsigned long *subkeys, int mode, 
        int length, unsigned char *input, unsigned char *output);


/* 
 * CBC模式SM4加解密函数
 * iv为128bits初始化向量
 */
void sm4_crypt_cbc( unsigned long *subkeys, int mode, int length,
        unsigned char iv[16], unsigned char *input, unsigned char *output );

/*
 * 计算加解密所用时间
 * key为初始128位密钥
 * 后面的参数参照sm4_crypt_cbc
 * 返回时间单位为us
 */
double cal_time( unsigned char *key, int mode, int length,
        unsigned char original_iv[16], unsigned char *input, 
        unsigned char *output );
#endif 