/*
 * SM4/SMS4 algorithm test programme
 * 2012-4-21
 */

#include <string.h>
#include <stdio.h>
#include "./sm4.h"

int main()
{
	unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char iv[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char output[16];
	unsigned long ctx[32];
	unsigned long i;
	unsigned char short_input[15] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32};
	unsigned char short_output[16];

    puts("key:");
	for(i=0;i<16;i++)
		printf("%02x", key[i]);
	printf("\n");

    puts("input:");
	for(i=0;i<16;i++)
		printf("%02x", input[i]);
	printf("\n");
    puts("iv:");
	for(i=0;i<16;i++)
		printf("%02x", iv[i]);
	printf("\n");

	//encrypt standard testing vector
	sm4_setkey(ctx,key, SM4_ENCRYPT);
	/*
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 16; j++) {
			printf("%02x ", ctx[j]);
		}
		putchar('\n');
	}
	*/
    puts("encrypt");
	sm4_crypt_cbc(ctx,1,16, iv, input,output);
	for(i=0;i<16;i++)
		printf("%02x", output[i]);
	printf("\n");

	//decrypt testing
	sm4_setkey(ctx,key, SM4_DECRYPT);
	sm4_crypt_cbc(ctx,0,16, iv,output,output);
	puts("decrypt");
	for(i=0;i<16;i++)
		printf("%02x", output[i]);
	printf("\n");

    puts("CalTime");
	double time_use;
	time_use = cal_time(key, 1, 16, iv, input, output);
	printf("time use is %lgus\n", time_use);

/*
	sm4_crypt_ecb(ctx,1,16,input,output);
	for(i=0;i<16;i++)
		printf("%02x ", output[i]);
	printf("\n");

	//decrypt testing
	sm4_setkey(ctx,key, SM4_DECRYPT);
	sm4_crypt_ecb(ctx,0,16,output,output);
	for(i=0;i<16;i++)
		printf("%02x ", output[i]);
	printf("\n");
	*/

/*
	//decrypt 1M times testing vector based on standards.
	i = 0;
	sm4_setkey(ctx,key, SM4_ENCRYPT);
	while (i<1000000) 
    {
		sm4_crypt_ecb(ctx,1,16,input,input);
		i++;
    }
	for(i=0;i<16;i++)
		printf("%02x ", input[i]);
	printf("\n");
	*/
	
	// short_block
	//encrypt standard testing vector
	sm4_setkey(ctx,key, SM4_ENCRYPT);
	/*
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 16; j++) {
			printf("%02x ", ctx[j]);
		}
		putchar('\n');
	}
	*/
    puts("encrypt");
	sm4_crypt_cbc(ctx,1,15, iv, short_input,short_output);
	for(i=0;i<16;i++)
		printf("%02x", short_output[i]);
	printf("\n");

	//decrypt testing
	sm4_setkey(ctx,key, SM4_DECRYPT);
	sm4_crypt_cbc(ctx,0,15, iv,short_output,short_output);
	puts("decrypt");
	for(i=0;i<15;i++)
		printf("%02x", short_output[i]);
	printf("\n");

    return 0;
}