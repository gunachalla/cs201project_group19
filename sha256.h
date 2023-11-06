#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stdlib.h>

// Define SHA-256 constants
#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

// Function to perform SHA-256 hash computation
void sha256(const uint8_t *data, size_t data_len, uint8_t *digest);

#endif // SHA256_H
