#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "./RC4.h"
#define N 80000
unsigned char htod(unsigned char hex);
unsigned char dtoh(unsigned char digit); 
void StringToBytes(char *str, unsigned char *bytes, int bytesSize);
void BytesToString(unsigned char *bytes,char *str, int bytesSize); 

char cipher_s[N], plain_s[N], keyseed_s[N];
int plainSize, keyseedSize;
unsigned char keyseed[N];
unsigned char plaintext[N], ciphertext[N];

int main() {
    FILE *inputptr, *outputptr;
    inputptr = fopen("in.txt", "r");
    outputptr = fopen("out.txt", "w");
    puts("Please input keyseeds");
    scanf("%s", keyseed_s);
    keyseedSize = strlen(keyseed_s)/2;
    StringToBytes(keyseed_s, keyseed, keyseedSize);
    
    char c;
    fgets(plain_s, N, inputptr);
    plainSize = strlen(plain_s)/2;
    StringToBytes(plain_s, plaintext, plainSize);
    RC4(plaintext, plainSize, ciphertext, keyseed, keyseedSize);
    BytesToString(ciphertext, cipher_s, plainSize);
    fputs(cipher_s, outputptr);
 
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

void StringToBytes(char *str, unsigned char *bytes, int bytesSize) {
    for (int i = 0; i < bytesSize; ++i) {
        bytes[i] = 16*htod(str[2*i]) + htod(str[2*i+1]);
    }
}

void BytesToString(unsigned char *bytes,char *str, int bytesSize) {
    for (int i = 0; i < bytesSize; ++i) {
        str[2*i] = dtoh((bytes[i]>>4) % 16);
        str[2*i+1] = dtoh(bytes[i]%16);
    }
}