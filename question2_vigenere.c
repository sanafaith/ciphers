/* Language: C */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * encrypt: Uses a Vigenere keyword to shift each alphabetic letter.
 * The keyword cycles to match the length of the plaintext.
 * Non-alphabetic characters are preserved unchanged.
 */
void encrypt(char *plaintext, const char *keyword) {
    int keyLen = strlen(keyword);
    int keyIndex = 0;

    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];
        if (isalpha((unsigned char)ch)) {
            int shift = toupper((unsigned char)keyword[keyIndex % keyLen]) - 'A';
            if (isupper((unsigned char)ch)) {
                plaintext[i] = ((ch - 'A' + shift + 26) % 26) + 'A';
            } else {
                plaintext[i] = ((ch - 'a' + shift + 26) % 26) + 'a';
            }
            keyIndex++;
        }
    }
}

/*
 * decrypt: Reverses the Vigenere encryption by subtracting keyword shifts.
 */
void decrypt(char *ciphertext, const char *keyword) {
    int keyLen = strlen(keyword);
    int keyIndex = 0;

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (isalpha((unsigned char)ch)) {
            int shift = toupper((unsigned char)keyword[keyIndex % keyLen]) - 'A';
            if (isupper((unsigned char)ch)) {
                ciphertext[i] = ((ch - 'A' - shift + 26) % 26) + 'A';
            } else {
                ciphertext[i] = ((ch - 'a' - shift + 26) % 26) + 'a';
            }
            keyIndex++;
        }
    }
}

int main(void) {
    char text[1024];
    char keyword[256];
    int choice;

    printf("Language: C\n");
    printf("Vigenere Cipher\n");
    printf("1. Encrypt plaintext\n");
    printf("2. Decrypt ciphertext\n");
    printf("Choose an option (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid selection.\n");
        return 1;
    }
    getchar();

    if (choice == 1) {
        printf("Enter plaintext: ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = '\0';

        printf("Enter keyword: ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        encrypt(text, keyword);
        printf("Ciphertext: %s\n", text);
    } else if (choice == 2) {
        printf("Enter ciphertext: ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = '\0';

        printf("Enter keyword: ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        decrypt(text, keyword);
        printf("Plaintext: %s\n", text);
    } else {
        printf("Invalid option.\n");
        return 1;
    }

    return 0;
}
