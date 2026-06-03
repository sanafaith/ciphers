/* Language: C */
#include <stdio.h>
#include <string.h>
#include <openssl/blowfish.h>

/*
 * encrypt: Uses Blowfish in ECB mode for a single 64-bit block.
 * The key may be 8-56 bytes (64-448 bits); we accept ASCII input.
 */
void encrypt(const unsigned char plaintext[8], const unsigned char *key,
             int keylen, unsigned char ciphertext[8], BF_KEY *bfkey) {
    BF_set_key(bfkey, keylen, key);
    BF_ecb_encrypt(plaintext, ciphertext, bfkey, BF_ENCRYPT);
}

void decrypt(const unsigned char ciphertext[8], const unsigned char *key,
             int keylen, unsigned char plaintext[8], BF_KEY *bfkey) {
    BF_set_key(bfkey, keylen, key);
    BF_ecb_encrypt(ciphertext, plaintext, bfkey, BF_DECRYPT);
}

void print_hex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char plaintext[8] = {0};
    unsigned char key[56] = {0};
    unsigned char ciphertext[8];
    unsigned char recovered[8];
    BF_KEY bfkey;
    int keylen;

    printf("Language: C\n");
    printf("Blowfish Cipher (OpenSSL)\n");
    printf("Enter 8-byte plaintext block: ");
    fgets((char *)plaintext, sizeof(plaintext) + 1, stdin);
    plaintext[strcspn((char *)plaintext, "\n")] = '\0';
    if (strlen((char *)plaintext) < 8) {
        memset(plaintext + strlen((char *)plaintext), ' ', 8 - strlen((char *)plaintext));
    }

    printf("Enter key (8-56 chars): ");
    fgets((char *)key, sizeof(key) + 1, stdin);
    keylen = strcspn((char *)key, "\n");
    key[keylen] = '\0';
    if (keylen < 8) {
        printf("Key must be at least 8 bytes.\n");
        return 1;
    }
    if (keylen > 56) keylen = 56;

    BF_set_key(&bfkey, keylen, key);
    printf("Blowfish P-array first 18 entries after key setup:\n");
    for (int i = 0; i < 18; i++) {
        printf("P[%02d] = %08X\n", i, bfkey.P[i]);
    }

    BF_ecb_encrypt(plaintext, ciphertext, &bfkey, BF_ENCRYPT);
    printf("Ciphertext (hex): ");
    print_hex(ciphertext, 8);

    BF_ecb_encrypt(ciphertext, recovered, &bfkey, BF_DECRYPT);
    printf("Recovered plaintext: '%.*s'\n", 8, recovered);
    return 0;
}
