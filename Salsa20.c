#include <stdio.h>
#include <stdint.h>

#define ROUNDS 20

// Define Salsa20 operations
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d)(    \
    b ^= ROTL(a + d, 7),   \
    c ^= ROTL(b + a, 9),   \
    d ^= ROTL(c + b,13),   \
    a ^= ROTL(d + c,18))

// Salsa20 block function
void salsa20_block(uint32_t out[16], uint32_t const in[16]) {
    int i;
    uint32_t x[16];

    for (i = 0; i < 16; ++i)
        x[i] = in[i];

    for (i = 0; i < ROUNDS; i += 2) {
        QR(x[ 0], x[ 4], x[ 8], x[12]);
        QR(x[ 5], x[ 9], x[13], x[ 1]);
        QR(x[10], x[14], x[ 2], x[ 6]);
        QR(x[15], x[ 3], x[ 7], x[11]);

        QR(x[ 0], x[ 1], x[ 2], x[ 3]);
        QR(x[ 5], x[ 6], x[ 7], x[ 4]);
        QR(x[10], x[11], x[ 8], x[ 9]);
        QR(x[15], x[12], x[13], x[14]);
    }

    for (i = 0; i < 16; ++i)
        out[i] = x[i] + in[i];
}

// Driver function
int main() {
    uint32_t plaintext[16];
    uint32_t ciphertext[16];

    // Prompt the user to enter the plaintext
    printf("Enter 64 bytes (16 integers) of plaintext:\n");
    for (int i = 0; i < 16; ++i) {
        if (scanf("%u", &plaintext[i]) != 1) {
            printf("Invalid input.\n");
            return 1;
        }
    }

    // Call the Salsa20 block function to encrypt the plaintext
    salsa20_block(ciphertext, plaintext);

    // Display the ciphertext
    printf("Ciphertext:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%u ", ciphertext[i]);
    }
    printf("\n");

    return 0;
}
