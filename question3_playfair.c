/* Language: C */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 5

/*
 * Build the 5x5 Playfair key matrix from the keyword.
 * I and J share a single cell, and duplicate letters are omitted.
 */
void build_matrix(const char *keyword, char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int seen[26] = {0};
    char flat[25];
    int count = 0;

    for (int i = 0; keyword[i] != '\0'; i++) {
        if (!isalpha((unsigned char)keyword[i])) continue;
        char ch = toupper((unsigned char)keyword[i]);
        if (ch == 'J') ch = 'I';
        int index = ch - 'A';
        if (!seen[index]) {
            seen[index] = 1;
            flat[count++] = ch;
        }
    }

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        int index = ch - 'A';
        if (!seen[index]) {
            flat[count++] = ch;
            seen[index] = 1;
        }
    }

    count = 0;
    for (int row = 0; row < MATRIX_SIZE; row++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            matrix[row][col] = flat[count++];
        }
    }
}

/*
 * Find the position of a letter in the Playfair matrix.
 */
void find_position(char matrix[MATRIX_SIZE][MATRIX_SIZE], char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int r = 0; r < MATRIX_SIZE; r++) {
        for (int c = 0; c < MATRIX_SIZE; c++) {
            if (matrix[r][c] == ch) {
                *row = r;
                *col = c;
                return;
            }
        }
    }
}

/*
 * Prepare a letter-only version of the input for Playfair encryption.
 * Repeated letters in a digraph are separated by 'X'.
 * If the length is odd, a trailing 'X' is appended.
 */
void prepare_text(const char *input, char *output) {
    int outIndex = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isalpha((unsigned char)input[i])) continue;
        char ch = toupper((unsigned char)input[i]);
        if (ch == 'J') ch = 'I';
        output[outIndex++] = ch;
    }
    output[outIndex] = '\0';

    char temp[1024];
    int tempIndex = 0;
    for (int i = 0; output[i] != '\0'; i += 2) {
        char first = output[i];
        char second = output[i + 1];
        temp[tempIndex++] = first;
        if (second == '\0') {
            temp[tempIndex++] = 'X';
            break;
        }
        if (first == second) {
            temp[tempIndex++] = 'X';
            i -= 1;
        } else {
            temp[tempIndex++] = second;
        }
    }
    temp[tempIndex] = '\0';
    strcpy(output, temp);
}

void encrypt(char *plaintext, char *ciphertext, char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    char prepared[1024];
    prepare_text(plaintext, prepared);
    int outIndex = 0;

    for (int i = 0; prepared[i] != '\0'; i += 2) {
        char a = prepared[i];
        char b = prepared[i + 1];
        int ar, ac, br, bc;
        find_position(matrix, a, &ar, &ac);
        find_position(matrix, b, &br, &bc);

        if (ar == br) {
            ciphertext[outIndex++] = matrix[ar][(ac + 1) % MATRIX_SIZE];
            ciphertext[outIndex++] = matrix[br][(bc + 1) % MATRIX_SIZE];
        } else if (ac == bc) {
            ciphertext[outIndex++] = matrix[(ar + 1) % MATRIX_SIZE][ac];
            ciphertext[outIndex++] = matrix[(br + 1) % MATRIX_SIZE][bc];
        } else {
            ciphertext[outIndex++] = matrix[ar][bc];
            ciphertext[outIndex++] = matrix[br][ac];
        }
    }
    ciphertext[outIndex] = '\0';
}

void decrypt(char *ciphertext, char *plaintext, char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int outIndex = 0;
    for (int i = 0; ciphertext[i] != '\0'; i += 2) {
        char a = ciphertext[i];
        char b = ciphertext[i + 1];
        int ar, ac, br, bc;
        find_position(matrix, a, &ar, &ac);
        find_position(matrix, b, &br, &bc);

        if (ar == br) {
            plaintext[outIndex++] = matrix[ar][(ac + MATRIX_SIZE - 1) % MATRIX_SIZE];
            plaintext[outIndex++] = matrix[br][(bc + MATRIX_SIZE - 1) % MATRIX_SIZE];
        } else if (ac == bc) {
            plaintext[outIndex++] = matrix[(ar + MATRIX_SIZE - 1) % MATRIX_SIZE][ac];
            plaintext[outIndex++] = matrix[(br + MATRIX_SIZE - 1) % MATRIX_SIZE][bc];
        } else {
            plaintext[outIndex++] = matrix[ar][bc];
            plaintext[outIndex++] = matrix[br][ac];
        }
    }
    plaintext[outIndex] = '\0';
}

void print_matrix(char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    printf("Playfair Key Matrix:\n");
    for (int r = 0; r < MATRIX_SIZE; r++) {
        for (int c = 0; c < MATRIX_SIZE; c++) {
            printf(" %c", matrix[r][c]);
        }
        printf("\n");
    }
}

int main(void) {
    char input[1024];
    char keyword[256];
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    char ciphertext[1024];
    char plaintext[1024];
    int choice;

    printf("Language: C\n");
    printf("Playfair Cipher\n");
    printf("Enter a keyword for the key matrix: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    build_matrix(keyword, matrix);
    print_matrix(matrix);

    printf("1. Encrypt message\n");
    printf("2. Decrypt message\n");
    printf("Choose an option (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid selection.\n");
        return 1;
    }
    getchar();

    if (choice == 1) {
        printf("Enter plaintext: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        encrypt(input, ciphertext, matrix);
        printf("Ciphertext digraphs: %s\n", ciphertext);
    } else if (choice == 2) {
        printf("Enter ciphertext (letters only): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        decrypt(input, plaintext, matrix);
        printf("Plaintext: %s\n", plaintext);
    } else {
        printf("Invalid option.\n");
        return 1;
    }

    return 0;
}
