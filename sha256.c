//#ifndef SHA256.h
//#define SHA256.h

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Define SHA-256 constants
#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

// Circular right shift macro
#define RIGHT_ROTATE(x, n) ((x) >> (n)) | ((x) << (32 - (n)))

// Initial hash values (first 32 bits of the fractional parts of the square roots of the first 8 prime numbers)
static const uint32_t initial_hash[] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// SHA-256 round constants
static const uint32_t round_constants[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Function to perform SHA-256 hash computation
void sha256(const uint8_t *data, size_t data_len, uint8_t *digest) {
    uint32_t working_hash[8];
    uint32_t w[64];
    size_t i;

    // Initialize working hash
    for (i = 0; i < 8; i++) {
        working_hash[i] = initial_hash[i];
    }

    // Process the data in 512-bit blocks
    for (i = 0; i < data_len; i += SHA256_BLOCK_SIZE) {
        const uint8_t *block = data + i;

        // Prepare message schedule
        for (size_t t = 0; t < 16; t++) {
            w[t] = (block[t * 4] << 24) | (block[t * 4 + 1] << 16) | (block[t * 4 + 2] << 8) | (block[t * 4 + 3]);
        }

        for (size_t t = 16; t < 64; t++) {
            uint32_t s0 = RIGHT_ROTATE(w[t - 15], 7) ^ RIGHT_ROTATE(w[t - 15], 18) ^ (w[t - 15] >> 3);
            uint32_t s1 = RIGHT_ROTATE(w[t - 2], 17) ^ RIGHT_ROTATE(w[t - 2], 19) ^ (w[t - 2] >> 10);
            w[t] = w[t - 16] + s0 + w[t - 7] + s1;
        }

        // Initialize working variables
        uint32_t a = working_hash[0];
        uint32_t b = working_hash[1];
        uint32_t c = working_hash[2];
        uint32_t d = working_hash[3];
        uint32_t e = working_hash[4];
        uint32_t f = working_hash[5];
        uint32_t g = working_hash[6];
        uint32_t h = working_hash[7];

        // Main loop
        for (size_t t = 0; t < 64; t++) {
            uint32_t temp1 = h + (RIGHT_ROTATE(e, 6) ^ RIGHT_ROTATE(e, 11) ^ RIGHT_ROTATE(e, 25)) + ((e & f) ^ ((~e) & g)) + round_constants[t] + w[t];
            uint32_t temp2 = (RIGHT_ROTATE(a, 2) ^ RIGHT_ROTATE(a, 13) ^ RIGHT_ROTATE(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // Update working hash
        working_hash[0] += a;
        working_hash[1] += b;
        working_hash[2] += c;
        working_hash[3] += d;
        working_hash[4] += e;
        working_hash[5] += f;
        working_hash[6] += g;
        working_hash[7] += h;
    }

    // Copy the final hash to the digest array in big-endian order
    for (i = 0; i < 8; i++) {
        digest[i * 4] = (working_hash[i] >> 24) & 0xFF;
        digest[i * 4 + 1] = (working_hash[i] >> 16) & 0xFF;
        digest[i * 4 + 2] = (working_hash[i] >> 8) & 0xFF;
        digest[i * 4 + 3] = working_hash[i] & 0xFF;
    }



       // Convert the binary digest to a hexadecimal string
    char hexDigest[2 * SHA256_DIGEST_SIZE + 1]; // +1 for the null terminator
    for (size_t i = 0; i < SHA256_DIGEST_SIZE; i++) {
        snprintf(&hexDigest[i * 2], 3, "%02x", digest[i]);
    }
    hexDigest[2 * SHA256_DIGEST_SIZE] = '\0'; // Null-terminate the string


    
}

/*int main(){
    uint8_t data[] = "Arya vardhan padal";
    size_t data_len = strlen((char *)data);
    uint8_t digest[SHA256_DIGEST_SIZE];

    sha256(data, data_len, digest);

      // Convert the binary digest to a hexadecimal string
    char hexDigest[2 * SHA256_DIGEST_SIZE + 1]; // +1 for the null terminator
    for (size_t i = 0; i < SHA256_DIGEST_SIZE; i++) {
        snprintf(&hexDigest[i * 2], 3, "%02x", digest[i]);
    }
    hexDigest[2 * SHA256_DIGEST_SIZE] = '\0'; // Null-terminate the string

    printf("SHA-256 Digest: %s\n", hexDigest);

    return 0;
}*/

