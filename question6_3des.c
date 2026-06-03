/* Language: C */
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

/*
 * encrypt: Applies 3DES in EDE mode: Encrypt with K1, Decrypt with K2, Encrypt with K3.
 * This improves security over single DES by effectively applying three independent keys.
 */
int encrypt(const unsigned char *plaintext, const unsigned char *key24, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int outLen = 0;
    int finalLen = 0;

    EVP_EncryptInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key24, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 0);
    EVP_EncryptUpdate(ctx, ciphertext, &outLen, plaintext, 8);
    EVP_EncryptFinal_ex(ctx, ciphertext + outLen, &finalLen);
    EVP_CIPHER_CTX_free(ctx);
    return outLen + finalLen;
}

int decrypt(const unsigned char *ciphertext, const unsigned char *key24, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int outLen = 0;
    int finalLen = 0;

    EVP_DecryptInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key24, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 0);
    EVP_DecryptUpdate(ctx, plaintext, &outLen, ciphertext, 8);
    EVP_DecryptFinal_ex(ctx, plaintext + outLen, &finalLen);
    EVP_CIPHER_CTX_free(ctx);
    return outLen + finalLen;
}

void print_hex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char plaintext[8] = {0};
    unsigned char key1[8] = {0};
    unsigned char key2[8] = {0};
    unsigned char key3[8] = {0};
    unsigned char key24[24];
    unsigned char ciphertext[8];
    unsigned char recovered[8];

    printf("Language: C\n");
    printf("Triple DES (3DES) Cipher\n");
    printf("Enter 8-byte plaintext block: ");
    fgets((char *)plaintext, sizeof(plaintext) + 1, stdin);
    plaintext[strcspn((char *)plaintext, "\n")] = '\0';
    if (strlen((char *)plaintext) < 8) {
        memset(plaintext + strlen((char *)plaintext), ' ', 8 - strlen((char *)plaintext));
    }

    printf("Enter key K1 (8 characters): ");
    fgets((char *)key1, sizeof(key1) + 1, stdin);
    key1[strcspn((char *)key1, "\n")] = '\0';
    if (strlen((char *)key1) < 8) memset(key1 + strlen((char *)key1), ' ', 8 - strlen((char *)key1));

    printf("Enter key K2 (8 characters): ");
    fgets((char *)key2, sizeof(key2) + 1, stdin);
    key2[strcspn((char *)key2, "\n")] = '\0';
    if (strlen((char *)key2) < 8) memset(key2 + strlen((char *)key2), ' ', 8 - strlen((char *)key2));

    printf("Enter key K3 (8 characters): ");
    fgets((char *)key3, sizeof(key3) + 1, stdin);
    key3[strcspn((char *)key3, "\n")] = '\0';
    if (strlen((char *)key3) < 8) memset(key3 + strlen((char *)key3), ' ', 8 - strlen((char *)key3));

    memcpy(key24, key1, 8);
    memcpy(key24 + 8, key2, 8);
    memcpy(key24 + 16, key3, 8);

    int encryptedLen = encrypt(plaintext, key24, ciphertext);
    printf("Encrypted ciphertext (hex): ");
    print_hex(ciphertext, encryptedLen);

    int decryptedLen = decrypt(ciphertext, key24, recovered);
    printf("Decrypted plaintext: '%.*s'\n", decryptedLen, recovered);
    return 0;
}
