/* Language: C */
#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

/*
 * encrypt: Encrypt a single 64-bit block using DES.
 * The DES key is supplied as an 8-byte value; DES uses 56 key bits internally.
 * Using OpenSSL DES here for the block cipher implementation.
 */
void encrypt(const unsigned char plaintext[8], const unsigned char key[8], unsigned char ciphertext[8]) {
    DES_cblock keyBlock;
    DES_key_schedule schedule;

    memcpy(keyBlock, key, 8);
    DES_set_key_unchecked(&keyBlock, &schedule);
    DES_ecb_encrypt((const_DES_cblock *)plaintext, (DES_cblock *)ciphertext, &schedule, DES_ENCRYPT);
}

/*
 * decrypt: Decrypt a single 64-bit ciphertext block.
 */
void decrypt(const unsigned char ciphertext[8], const unsigned char key[8], unsigned char plaintext[8]) {
    DES_cblock keyBlock;
    DES_key_schedule schedule;

    memcpy(keyBlock, key, 8);
    DES_set_key_unchecked(&keyBlock, &schedule);
    DES_ecb_encrypt((const_DES_cblock *)ciphertext, (DES_cblock *)plaintext, &schedule, DES_DECRYPT);
}

void print_block_hex(const unsigned char block[8]) {
    for (int i = 0; i < 8; i++) {
        printf("%02X", block[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char plaintext[8] = {0};
    unsigned char key[8] = {0};
    unsigned char ciphertext[8];
    unsigned char recovered[8];

    printf("Language: C\n");
    printf("DES Cipher (OpenSSL DES, ECB mode)\n");
    printf("Enter 8-byte plaintext block (exactly 8 characters): ");
    fgets((char *)plaintext, sizeof(plaintext) + 1, stdin);
    plaintext[strcspn((char *)plaintext, "\n")] = '\0';
    if (strlen((char *)plaintext) < 8) {
        memset(plaintext + strlen((char *)plaintext), ' ', 8 - strlen((char *)plaintext));
    }

    printf("Enter 8-byte key (use 8 ASCII characters): ");
    fgets((char *)key, sizeof(key) + 1, stdin);
    key[strcspn((char *)key, "\n")] = '\0';
    if (strlen((char *)key) < 8) {
        memset(key + strlen((char *)key), ' ', 8 - strlen((char *)key));
    }

    encrypt(plaintext, key, ciphertext);
    printf("Ciphertext (hex): ");
    print_block_hex(ciphertext);

    decrypt(ciphertext, key, recovered);
    printf("Decrypted plaintext: '%.*s'\n", 8, recovered);
    return 0;
}
