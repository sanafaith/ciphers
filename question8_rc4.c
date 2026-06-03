/* Language: C */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void ksa(const unsigned char *key, int keylen, unsigned char *S) {
    for (int i = 0; i < 256; i++) {
        S[i] = (unsigned char)i;
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keylen]) & 0xFF;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

void prga(unsigned char *S, unsigned char *stream, int length) {
    int i = 0, j = 0;
    for (int k = 0; k < length; k++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        stream[k] = S[(S[i] + S[j]) & 0xFF];
    }
}

void rc4_encrypt_decrypt(const unsigned char *input, int length,
                         const unsigned char *key, int keylen,
                         unsigned char *output, unsigned char *stream) {
    unsigned char S[256];
    ksa(key, keylen, S);
    prga(S, stream, length);
    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ stream[i];
    }
}

void print_hex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    char message[1024];
    char keyInput[256];
    unsigned char output[1024];
    unsigned char stream[1024];
    int length;

    printf("Language: C\n");
    printf("RC4 Stream Cipher\n");
    printf("Enter key: ");
    fgets(keyInput, sizeof(keyInput), stdin);
    keyInput[strcspn(keyInput, "\n")] = '\0';

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);
    length = strcspn(message, "\n");
    message[length] = '\0';

    rc4_encrypt_decrypt((unsigned char *)message, length,
                        (unsigned char *)keyInput, strlen(keyInput),
                        output, stream);

    printf("Keystream bytes (hex): ");
    print_hex(stream, length);
    printf("Ciphertext (hex): ");
    print_hex(output, length);

    unsigned char recovered[1024];
    unsigned char stream2[1024];
    rc4_encrypt_decrypt(output, length,
                        (unsigned char *)keyInput, strlen(keyInput),
                        recovered, stream2);
    recovered[length] = '\0';
    printf("Recovered plaintext: %s\n", recovered);
    return 0;
}
