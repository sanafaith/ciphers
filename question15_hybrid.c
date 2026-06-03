/* Language: C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/err.h>

void print_hex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) printf("%02X", data[i]);
    printf("\n");
}

int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, ciphertext_len;
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, plaintext_len;
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

int main(void) {
    char message[1024];
    unsigned char session_key[16];
    unsigned char iv[16];
    unsigned char ciphertext[2048];
    unsigned char decrypted[2048];
    unsigned char encrypted_session_key[256];
    unsigned char decrypted_session_key[256];
    int ciphertext_len, decrypted_len, encrypted_key_len;
    RSA *rsa = NULL;
    BIGNUM *bn = NULL;

    printf("Language: C\n");
    printf("Hybrid Encryption: RSA + AES-128-CBC\n");
    printf("Enter plaintext message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    if (!RAND_bytes(session_key, sizeof(session_key)) || !RAND_bytes(iv, sizeof(iv))) {
        fprintf(stderr, "Failed to generate random key or IV.\n");
        return 1;
    }

    bn = BN_new();
    BN_set_word(bn, RSA_F4);
    rsa = RSA_new();
    RSA_generate_key_ex(rsa, 2048, bn, NULL);
    BN_free(bn);

    encrypted_key_len = RSA_public_encrypt(sizeof(session_key), session_key,
                                           encrypted_session_key, rsa, RSA_PKCS1_OAEP_PADDING);
    if (encrypted_key_len == -1) {
        fprintf(stderr, "RSA public encrypt failed.\n");
        ERR_print_errors_fp(stderr);
        RSA_free(rsa);
        return 1;
    }

    ciphertext_len = aes_encrypt((unsigned char *)message, strlen(message),
                                 session_key, iv, ciphertext);

    printf("Session key (hex): "); print_hex(session_key, sizeof(session_key));
    printf("IV (hex): "); print_hex(iv, sizeof(iv));
    printf("Encrypted session key (hex): "); print_hex(encrypted_session_key, encrypted_key_len);
    printf("AES ciphertext (hex): "); print_hex(ciphertext, ciphertext_len);

    int decrypted_key_len = RSA_private_decrypt(encrypted_key_len, encrypted_session_key,
                                               decrypted_session_key, rsa, RSA_PKCS1_OAEP_PADDING);
    if (decrypted_key_len != sizeof(session_key)) {
        fprintf(stderr, "RSA private decrypt failed.\n");
        ERR_print_errors_fp(stderr);
        RSA_free(rsa);
        return 1;
    }

    decrypted_len = aes_decrypt(ciphertext, ciphertext_len, decrypted_session_key, iv, decrypted);
    decrypted[decrypted_len] = '\0';
    printf("Recovered plaintext: %s\n", decrypted);

    RSA_free(rsa);
    return 0;
}
