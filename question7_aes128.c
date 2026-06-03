/* Language: C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

/*
 * encrypt: AES-128-CBC with PKCS#7 padding.
 * Uses OpenSSL EVP for the AES transformation and handles padding automatically.
 */
int encrypt(const unsigned char *plaintext, int plaintext_len,
            const unsigned char *key, const unsigned char *iv,
            unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0;
    int ciphertext_len = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(const unsigned char *ciphertext, int ciphertext_len,
            const unsigned char *key, const unsigned char *iv,
            unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0;
    int plaintext_len = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

void print_hex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    char message[1024];
    unsigned char key[16];
    unsigned char iv[16];
    unsigned char ciphertext[2048];
    unsigned char decrypted[2048];
    int ciphertext_len, decrypted_len;
    int useProvidedIv;

    printf("Language: C\n");
    printf("AES-128 CBC Cipher (OpenSSL)\n");
    printf("Enter plaintext: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    printf("Enter 16-character AES key: ");
    fgets((char *)key, sizeof(key) + 1, stdin);
    key[strcspn((char *)key, "\n")] = '\0';
    if (strlen((char *)key) < 16) {
        memset(key + strlen((char *)key), ' ', 16 - strlen((char *)key));
    }

    printf("Enter 16-character IV or leave blank to generate one: ");
    fgets((char *)iv, sizeof(iv) + 1, stdin);
    int ivLen = strcspn((char *)iv, "\n");
    if (ivLen == 0) {
        RAND_bytes(iv, sizeof(iv));
        printf("Generated IV (hex): ");
        print_hex(iv, sizeof(iv));
    } else {
        iv[ivLen] = '\0';
        if (ivLen < 16) {
            memset(iv + ivLen, ' ', 16 - ivLen);
        }
    }

    ciphertext_len = encrypt((unsigned char *)message, strlen(message), key, iv, ciphertext);
    printf("Ciphertext (hex): ");
    print_hex(ciphertext, ciphertext_len);

    decrypted_len = decrypt(ciphertext, ciphertext_len, key, iv, decrypted);
    decrypted[decrypted_len] = '\0';
    printf("Decrypted output: %s\n", decrypted);
    return 0;
}
