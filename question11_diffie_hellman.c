/* Language: C */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t result = 0;
    a %= mod;
    while (b > 0) {
        if (b & 1) {
            result = (result + a) % mod;
        }
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
}

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

void caesar_encrypt(char *text, int key) {
    key %= 26;
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isupper((unsigned char)ch)) text[i] = ((ch - 'A' + key + 26) % 26) + 'A';
        else if (islower((unsigned char)ch)) text[i] = ((ch - 'a' + key + 26) % 26) + 'a';
    }
}

void caesar_decrypt(char *text, int key) {
    caesar_encrypt(text, -key);
}

int main(void) {
    uint64_t p, g, a, b;
    uint64_t A, B, sharedA, sharedB;
    char message[1024];

    printf("Language: C\n");
    printf("Diffie-Hellman Key Exchange\n");
    printf("Enter prime p: ");
    scanf("%llu", &p);
    printf("Enter primitive root g: ");
    scanf("%llu", &g);
    printf("Enter Alice's private key a: ");
    scanf("%llu", &a);
    printf("Enter Bob's private key b: ");
    scanf("%llu", &b);
    getchar();

    A = modexp(g, a, p);
    B = modexp(g, b, p);
    printf("Alice public A = g^a mod p = %llu\n", A);
    printf("Bob public B = g^b mod p = %llu\n", B);

    sharedA = modexp(B, a, p);
    sharedB = modexp(A, b, p);
    printf("Alice shared secret = %llu\n", sharedA);
    printf("Bob shared secret = %llu\n", sharedB);

    if (sharedA != sharedB) {
        printf("Shared secrets do not match.\n");
        return 1;
    }

    printf("Enter a short message to encrypt with the shared secret: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    int key = sharedA % 26;
    caesar_encrypt(message, key);
    printf("Encrypted message using Caesar with key %d: %s\n", key, message);

    caesar_decrypt(message, key);
    printf("Decrypted back: %s\n", message);
    return 0;
}
