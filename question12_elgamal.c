/* Language: C */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

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

uint64_t modinv(uint64_t x, uint64_t mod) {
    uint64_t a = x, b = mod;
    int64_t u = 1, v = 0;
    while (b != 0) {
        uint64_t q = a / b;
        uint64_t r = a % b;
        a = b;
        b = r;
        int64_t tmp = u - (int64_t)q * v;
        u = v;
        v = tmp;
    }
    if (u < 0) u += mod;
    return u;
}

int main(void) {
    uint64_t p, g, x, y, plaintext;
    uint64_t k, c1, c2, s, s_inv, recovered;

    srand((unsigned int)time(NULL));
    printf("Language: C\n");
    printf("El-Gamal Encryption\n");
    printf("Enter prime p: ");
    scanf("%llu", &p);
    printf("Enter generator g: ");
    scanf("%llu", &g);
    printf("Enter private key x: ");
    scanf("%llu", &x);
    printf("Enter plaintext numeric message (0 < m < p): ");
    scanf("%llu", &plaintext);

    y = modexp(g, x, p);
    printf("Public key y = g^x mod p = %llu\n", y);

    k = (rand() % (p - 2)) + 1;
    printf("Random ephemeral k = %llu\n", k);
    c1 = modexp(g, k, p);
    s = modexp(y, k, p);
    c2 = mulmod(plaintext, s, p);

    printf("c1 = %llu\n", c1);
    printf("c2 = %llu\n", c2);
    printf("Shared secret y^k mod p = %llu\n", s);

    s_inv = modinv(s, p);
    printf("Modular inverse of shared secret = %llu\n", s_inv);

    recovered = mulmod(c2, s_inv, p);
    printf("Recovered plaintext: %llu\n", recovered);
    return 0;
}
