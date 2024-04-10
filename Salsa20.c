#include <stdio.h> // Include necessary header for input/output operations
#include <stdint.h> // Include necessary header for uint32_t data type

#define ROUNDS 20 

// Define Salsa20 operations
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b)))) 
#define QR(a, b, c, d) \
    b ^= ROTL(a + d, 7), \
    c ^= ROTL(b + a, 9), \
    d ^= ROTL(c + b, 13), \
    a ^= ROTL(d + c, 18)

// Salsa20 block function
void salsa20_block(uint32_t out[16], uint32_t const in[16], uint32_t const key[8], uint32_t const nonce[2], char flag) {
    int i;
    uint32_t x[16];
    // Set initial counter values
    uint32_t counter[2] = {0, 0}; // For example, initialize with zeros

    // Set initial values based on Salsa20 specification
    x[ 0] = 0x61707865;     x[ 1] = key[0];     x[ 2] = key[1];     x[ 3] = key[2]; 
	x[ 4] = key[3];         x[ 5] = 0x3320646e; x[ 6] = nonce[0];   x[ 7] = nonce[1];
	x[ 8] = counter[0];     x[ 9] = counter[1]; x[10] = 0x79622d32; x[11] = key[4]; 
    x[ 12] = key[5];        x[ 13] = key[6];    x[14] = key[7];     x[15] = 0x6b206574;

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

    if (flag == 1){
        // ENCRYPTION PART
        for (i = 0; i < 16; ++i)
            out[i] = x[i] + in[i];
    }
    else{
        // DECRYPTION PART
        for (i = 0; i < 16; ++i)
            out[i] = in[i] - x[i];
    }

}

// Validate input for key, nonce, and message
int validate_input(uint32_t input[16], int t) {
    for (int i = 0; i < t; ++i) {
        if (scanf("%u", &input[i]) != 1) {
            printf("Invalid input. Please enter Correct Number of integers.\n");
            return 0; // Return 0 to indicate failure
        }
    }
    return 1; // Return 1 to indicate success
}

// Driver function
int main() {
    char flag;
    uint32_t key[8];
    uint32_t nonce[2];
    uint32_t input[16];
    uint32_t output[16];

    // Prompt the user to enter the key
    printf("Enter 256-bit (8 integers) key:\n");
    if (!validate_input(key,8)) {
        return 1; // Exit if input is invalid
    }

    // Prompt the user to enter the nonce
    printf("Enter 64-bit (2 integers) nonce:\n");
    if (!validate_input(nonce,2)) {
        return 1; // Exit if input is invalid
    }

    // Encrypt/Decrypt
    printf("\nEnter 1 to Encrypt / 0 to Decrypt :");
    if (scanf("%c", &flag) == 1) {
        // Prompt the user to enter the Plaintext
        printf("\nEnter 64 bytes (16 integers) of Plaintext:\n");
        if (!validate_input(input,16)) {
            return 1; // Exit if input is invalid
        }
    }
    else{
        // Prompt the user to enter the Ciphertext
        printf("\nEnter 64 bytes (16 integers) of Ciphertext:\n");
        if (!validate_input(input,16)) {
            return 1; // Exit if input is invalid
        }
    }

    // Call the Salsa20 block function to encrypt the plaintext
    salsa20_block(output, input, key, nonce, flag);

    // Display the ciphertext
    printf("\nRESULT :\n");
    for (int i = 0; i < 16; ++i) {
        printf("%u ", output[i]); // Output the ciphertext
    }
    printf("\n");

    return 0; // Return 0 to indicate successful execution
}