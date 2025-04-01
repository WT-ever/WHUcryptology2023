#include <bits/stdc++.h>
#include "./aess.h"
using namespace std;

int main() {
    char key_s[33], plains[33];
    scanf("%s", key_s);
    scanf("%s", plains);
    unsigned char K[16], ciphertext[16], plaintext[16];
    unsigned char keys[11][16];
    StringToBytes(plains, plaintext, 16);
    StringToBytes(key_s, K, 16);
    keyExpansion(K, keys);
    for (int i = 0; i < 11; i++) {
        cout << i << ": ";
        for (int j = 0; j < 16; j++) {
            printf("%d ", htod(keys[i][j]));
        } 
        cout <<endl;
    }
    AES(plaintext, ciphertext, keys, 10);
    return 0;
}