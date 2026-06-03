/* Language: C */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
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

int64_t extended_gcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t d = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return d;
}

uint64_t mod_inverse(uint64_t e, uint64_t phi) {
    int64_t x, y;
    int64_t d = extended_gcd((int64_t)e, (int64_t)phi, &x, &y);
    if (d != 1) return 0;
    int64_t inv = x % (int64_t)phi;
    if (inv < 0) inv += phi;
    return inv;
}

int main(void) {
    uint64_t p, q, n, phi, e, d, plaintext, ciphertext, recovered;

    printf("Language: C\n");
    printf("RSA Key Generation and Encryption/Decryption\n");
    printf("Enter prime p: ");
    scanf("%llu", &p);
    printf("Enter prime q: ");
    scanf("%llu", &q);

    n = p * q;
    phi = (p - 1) * (q - 1);
    e = 65537;
    if (gcd(e, phi) != 1) {
        e = 17;
        while (e < phi && gcd(e, phi) != 1) {
            e += 2;
        }
    }
    d = mod_inverse(e, phi);
    if (d == 0) {
        printf("Unable to compute private exponent.\n");
        return 1;
    }

    printf("Public key (e, n): (%llu, %llu)\n", e, n);
    printf("Private key (d, n): (%llu, %llu)\n", d, n);

    printf("Enter plaintext numeric message (0 < m < n): ");
    scanf("%llu", &plaintext);
    if (plaintext >= n) {
        printf("Plaintext must be smaller than n.\n");
        return 1;
    }

    ciphertext = modexp(plaintext, e, n);
    printf("Ciphertext: %llu\n", ciphertext);

    recovered = modexp(ciphertext, d, n);
    printf("Recovered plaintext: %llu\n", recovered);
    return 0;
}
