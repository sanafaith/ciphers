/* Language: C */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * encrypt: Converts plaintext (PT) into ciphertext (CT)
 * using a Caesar shift cipher with a user-provided integer key.
 * The key schedule is simple: the key is an integer shift value
 * normalized modulo 26 to map into the 26 letters of the English alphabet.
 * Only alphabetic characters are shifted; other characters remain unchanged.
 */
void encrypt(char *plaintext, int key) {
    key = key % 26;  // Normalize key to the 0-25 range

    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];

        if (isupper((unsigned char)ch)) {
            plaintext[i] = ((ch - 'A' + key + 26) % 26) + 'A';
        } else if (islower((unsigned char)ch)) {
            plaintext[i] = ((ch - 'a' + key + 26) % 26) + 'a';
        }
    }
}

/*
 * decrypt: Converts ciphertext (CT) back to plaintext (PT)
 * by shifting letters in the opposite direction using the same key.
 */
void decrypt(char *ciphertext, int key) {
    encrypt(ciphertext, -key);
}

int main(void) {
    char input[1000];
    int key;
    int choice;

    printf("Caesar Cipher Program\n");
    printf("1. Encrypt plaintext\n");
    printf("2. Decrypt ciphertext\n");
    printf("Choose an option (1 or 2): ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid selection. Exiting.\n");
        return 1;
    }

    getchar();

    if (choice == 1) {
        printf("Enter plaintext: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Exiting.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        printf("Enter key (integer shift value): ");
        if (scanf("%d", &key) != 1) {
            printf("Invalid key. Exiting.\n");
            return 1;
        }

        encrypt(input, key);
        printf("Ciphertext: %s\n", input);
    } else if (choice == 2) {
        printf("Enter ciphertext: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Exiting.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        printf("Enter key (integer shift value): ");
        if (scanf("%d", &key) != 1) {
            printf("Invalid key. Exiting.\n");
            return 1;
        }

        decrypt(input, key);
        printf("Plaintext: %s\n", input);
    } else {
        printf("Invalid option. Exiting.\n");
        return 1;
    }

    return 0;
}
