/* Language: C */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Simplified ECDH demonstration on a small toy curve over Fp.
 * Curve: y^2 = x^3 + ax + b (mod p).
 */

uint64_t modinv(uint64_t a, uint64_t mod) {
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0;
    if (t < 0) t += mod;
    return t;
}

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

typedef struct { uint64_t x, y; int infinity; } Point;

Point point_add(Point P, Point Q, uint64_t a, uint64_t p) {
    if (P.infinity) return Q;
    if (Q.infinity) return P;
    if (P.x == Q.x && (P.y + Q.y) % p == 0) return (Point){0, 0, 1};

    uint64_t lambda;
    if (P.x == Q.x && P.y == Q.y) {
        uint64_t xx = mulmod(P.x, P.x, p);
        uint64_t numerator = (mulmod(3, xx, p) + a) % p;
        uint64_t denominator = (2 * P.y) % p;
        lambda = mulmod(numerator, modinv(denominator, p), p);
    } else {
        uint64_t numerator = (Q.y + p - P.y) % p;
        uint64_t denominator = (Q.x + p - P.x) % p;
        lambda = mulmod(numerator, modinv(denominator, p), p);
    }

    uint64_t xr = (mulmod(lambda, lambda, p) + p - P.x + p - Q.x) % p;
    uint64_t yr = (mulmod(lambda, (P.x + p - xr) % p, p) + p - P.y) % p;
    return (Point){xr, yr, 0};
}

Point scalar_mul(Point P, uint64_t k, uint64_t a, uint64_t p) {
    Point R = {0, 0, 1};
    while (k > 0) {
        if (k & 1) R = point_add(R, P, a, p);
        P = point_add(P, P, a, p);
        k >>= 1;
    }
    return R;
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
    uint64_t p = 97;
    uint64_t a = 2;
    uint64_t b = 3;
    Point G = {3, 6, 0};
    uint64_t privA, privB;
    char message[1024];

    printf("Language: C\n");
    printf("Simplified ECC Diffie-Hellman\n");
    printf("Using curve y^2 = x^3 + 2x + 3 over F_%llu\n", p);
    printf("Base point G = (%llu, %llu)\n", G.x, G.y);

    printf("Enter Alice private key: ");
    scanf("%llu", &privA);
    printf("Enter Bob private key: ");
    scanf("%llu", &privB);
    getchar();

    Point pubA = scalar_mul(G, privA, a, p);
    Point pubB = scalar_mul(G, privB, a, p);
    printf("Alice public key: (%llu, %llu)\n", pubA.x, pubA.y);
    printf("Bob public key: (%llu, %llu)\n", pubB.x, pubB.y);

    Point sharedA = scalar_mul(pubB, privA, a, p);
    Point sharedB = scalar_mul(pubA, privB, a, p);
    printf("Alice computed shared point: (%llu, %llu)\n", sharedA.x, sharedA.y);
    printf("Bob computed shared point: (%llu, %llu)\n", sharedB.x, sharedB.y);

    if (sharedA.infinity || sharedB.infinity || sharedA.x != sharedB.x || sharedA.y != sharedB.y) {
        printf("Shared secret mismatch.\n");
        return 1;
    }

    printf("Enter message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    int key = sharedA.x % 26;
    caesar_encrypt(message, key);
    printf("Encrypted message using derived key %d: %s\n", key, message);

    caesar_decrypt(message, key);
    printf("Recovered plaintext: %s\n", message);
    return 0;
}
