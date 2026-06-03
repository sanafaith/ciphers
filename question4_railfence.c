/* Language: C */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * encrypt: Arrange plaintext in a zigzag over n rails and read each rail.
 */
void encrypt(const char *plaintext, char *ciphertext, int rails) {
    int length = strlen(plaintext);
    int index = 0;

    char pattern[rails][length];
    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < length; c++) {
            pattern[r][c] = '\n';
        }
    }

    int row = 0;
    int direction = 1;
    for (int i = 0; i < length; i++) {
        pattern[row][i] = plaintext[i];
        row += direction;
        if (row == rails) {
            row = rails - 2;
            direction = -1;
        } else if (row == -1) {
            row = 1;
            direction = 1;
        }
    }

    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < length; c++) {
            if (pattern[r][c] != '\n') {
                ciphertext[index++] = pattern[r][c];
            }
        }
    }
    ciphertext[index] = '\0';
}

/*
 * decrypt: Reconstruct the zigzag pattern and read plaintext in original order.
 */
void decrypt(const char *ciphertext, char *plaintext, int rails) {
    int length = strlen(ciphertext);
    char marker[rails][length];
    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < length; c++) {
            marker[r][c] = '\n';
        }
    }

    int row = 0;
    int direction = 1;
    for (int i = 0; i < length; i++) {
        marker[row][i] = '*';
        row += direction;
        if (row == rails) {
            row = rails - 2;
            direction = -1;
        } else if (row == -1) {
            row = 1;
            direction = 1;
        }
    }

    int idx = 0;
    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < length; c++) {
            if (marker[r][c] == '*') {
                marker[r][c] = ciphertext[idx++];
            }
        }
    }

    row = 0;
    direction = 1;
    for (int i = 0; i < length; i++) {
        plaintext[i] = marker[row][i];
        row += direction;
        if (row == rails) {
            row = rails - 2;
            direction = -1;
        } else if (row == -1) {
            row = 1;
            direction = 1;
        }
    }
    plaintext[length] = '\0';
}

void print_rail_diagram(const char *text, int rails) {
    int length = strlen(text);
    char diagram[rails][length];
    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < length; c++) {
            diagram[r][c] = ' ';
        }
    }

    int row = 0;
    int direction = 1;
    for (int i = 0; i < length; i++) {
        diagram[row][i] = text[i];
        row += direction;
        if (row == rails) {
            row = rails - 2;
            direction = -1;
        } else if (row == -1) {
            row = 1;
            direction = 1;
        }
    }

    printf("Rail Fence diagram:\n");
    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < length; c++) {
            putchar(diagram[r][c]);
        }
        putchar('\n');
    }
}

int main(void) {
    char input[1024];
    char output[1024];
    int rails;
    int choice;

    printf("Language: C\n");
    printf("Rail Fence Cipher\n");
    printf("1. Encrypt plaintext\n");
    printf("2. Decrypt ciphertext\n");
    printf("Choose an option (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid selection.\n");
        return 1;
    }
    getchar();

    printf("Enter number of rails: ");
    if (scanf("%d", &rails) != 1 || rails < 2) {
        printf("Invalid rail count.\n");
        return 1;
    }
    getchar();

    if (choice == 1) {
        printf("Enter plaintext: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        print_rail_diagram(input, rails);
        encrypt(input, output, rails);
        printf("Ciphertext: %s\n", output);
    } else if (choice == 2) {
        printf("Enter ciphertext: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        decrypt(input, output, rails);
        printf("Plaintext: %s\n", output);
    } else {
        printf("Invalid option.\n");
        return 1;
    }

    return 0;
}
