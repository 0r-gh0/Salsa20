#include <stdio.h> // Include necessary header for input/output operations
#include <stdint.h> // Include necessary header for uint32_t data type

#define ROUNDS 20 

// Define Salsa20 operations
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b)))) 
#define QR(a, b, c, d)(    \    
	b ^= ROTL(a + d, 7),   \ 
    c ^= ROTL(b + a, 9),   \ 
    d ^= ROTL(c + b,13),   \ 
    a ^= ROTL(d + c,18))

// Salsa20 block function
void salsa20_block(uint32_t out[16], uint32_t const in[16], uint32_t const key[8], uint32_t const nonce[2]) {
    int i;
    uint32_t x[16];

    // Set initial values based on Salsa20 specification
    x[ 0] = 0x61707865; x[ 1] = key[0]; x[ 2] = key[1]; x[ 3] = key[2]; x[ 4] = key[3];
    x[ 5] = 0x3320646e; x[ 6] = nonce[0]; x[ 7] = nonce[1]; x[ 8] = key[4]; x[ 9] = key[5];
    x[10] = key[6]; x[11] = key[7]; x[12] = 0x79622d32; x[13] = 0x6b206574; x[14] = 0x6e65706f;
    x[15] = 0x20202020;

    for (i = 0; i < 16; ++i)
        x[i] = in[i]; // Copy input to temporary array

    // Perform Salsa20 rounds
    for (i = 0; i < ROUNDS; i += 2) {
        // Odd round: Perform quarter-round operations on columns
        QR(x[ 0], x[ 4], x[ 8], x[12]);
        QR(x[ 5], x[ 9], x[13], x[ 1]);
        QR(x[10], x[14], x[ 2], x[ 6]);
        QR(x[15], x[ 3], x[ 7], x[11]);

        // Even round: Perform quarter-round operations on rows
        QR(x[ 0], x[ 1], x[ 2], x[ 3]);
        QR(x[ 5], x[ 6], x[ 7], x[ 4]);
        QR(x[10], x[11], x[ 8], x[ 9]);
        QR(x[15], x[12], x[13], x[14]);
    }

    // Add original input to the result
    for (i = 0; i < 16; ++i)
        out[i] = x[i] + in[i];
}

// Driver function
int main() {
    uint32_t plaintext[16]; // Array to store plaintext
    uint32_t ciphertext[16]; // Array to store ciphertext
    uint32_t key[8]; // Array to store key
    uint32_t nonce[2]; // Array to store nonce

    // Prompt the user to enter the key
    printf("Enter 256-bit (8 integers) key:\n");
    for (int i = 0; i < 8; ++i) {
        if (scanf("%u", &key[i]) != 1) { // Read key input
            printf("Invalid input.\n");
            return 1;
        }
    }

    // Prompt the user to enter the nonce
    printf("Enter 64-bit (2 integers) nonce:\n");
    for (int i = 0; i < 2; ++i) {
        if (scanf("%u", &nonce[i]) != 1) { // Read nonce input
            printf("Invalid input.\n");
            return 1;
        }
    }

    // Prompt the user to enter the plaintext
    printf("Enter 64 bytes (16 integers) of plaintext:\n");
    for (int i = 0; i < 16; ++i) {
        if (scanf("%u", &plaintext[i]) != 1) { // Read plaintext input
            printf("Invalid input.\n");
            return 1;
        }
    }

    // Call the Salsa20 block function to encrypt the plaintext
    salsa20_block(ciphertext, plaintext, key, nonce);

    // Display the ciphertext
    printf("Ciphertext:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%u ", ciphertext[i]); // Output the ciphertext
    }
    printf("\n");

    return 0; // Return 0 to indicate successful execution
}
