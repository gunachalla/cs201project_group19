#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SHA256.h"  // Assuming SHA256 functions are defined in "SHA256.h" header

#define SHA256_HASH_SIZE 32  // Size of the SHA-256 hash in bytes

struct hashnode {
    char hash[65];  // String to store a hexadecimal hash (plus null terminator)
};

typedef struct hashnode hash;

// Function to compute Merkle tree hashes from leaf nodes
void computeHashArray(hash* hasharray, int numLeafNodes) {
    if (numLeafNodes <= 0) {
        return;
    }

    while (numLeafNodes > 1) {
        int parentIndex = 0;
        for (int i = 0; i < numLeafNodes; i += 2) {
            // Concatenate and hash the current pair of leaf nodes
            char concatenated[2 * SHA256_HASH_SIZE + 1];
            strcpy(concatenated, hasharray[i].hash);
            if (i + 1 < numLeafNodes) {
                strcat(concatenated, hasharray[i + 1].hash);
            }

            uint8_t hashResult[SHA256_HASH_SIZE];
            sha256((uint8_t*)concatenated, strlen(concatenated), hashResult);

            // Convert the hash to a hexadecimal string
            char hexDigest[2 * SHA256_HASH_SIZE + 1];
            for (size_t j = 0; j < SHA256_HASH_SIZE; j++) {
                snprintf(&hexDigest[j * 2], 3, "%02x", hashResult[j]);
            }
            hexDigest[2 * SHA256_HASH_SIZE] = '\0';

            // Update the hasharray with the new hash
            strcpy(hasharray[parentIndex].hash, hexDigest);
            parentIndex++;
        }

        // If there's an odd number of nodes, duplicate the last one
        if (numLeafNodes % 2 != 0) {
            // Assuming hasharray[parentIndex] is already initialized
            strcpy(hasharray[parentIndex].hash, hasharray[numLeafNodes - 1].hash);
            parentIndex++;
        }

        numLeafNodes = parentIndex;
    }
}


int main() {
    FILE* hashFile;
    int numLeafNodes = 0;

    hashFile = fopen("party1.txt", "r");

    if (hashFile == NULL) {
        printf("Failed to open the file for reading.\n");
        return 1;
    }

    // Determine the number of leaf nodes
    while (numLeafNodes < 64) {
        char line[66];  // Assuming each line in hash1.txt has 65 characters

        if (fscanf(hashFile, "%65s", line) != 1) {
            break;
        }

        numLeafNodes++;
    }

    fclose(hashFile);

    // Allocate memory for the hasharray
    hash* hasharray = (hash*)malloc(numLeafNodes * sizeof(hash));
    if (hasharray == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Read the leaf nodes from hash1.txt
    hashFile = fopen("party1.txt", "r");

    int i = 0;
    while (i < numLeafNodes && fscanf(hashFile, "%64s", hasharray[i].hash) == 1) {
        i++;
    }

    fclose(hashFile);

    // Print the input leaf hashes
    printf("Input Leaf Hashes:\n");
    for (i = 0; i < numLeafNodes; i++) {
        printf("Node %d: %s\n", i, hasharray[i].hash);
    }

    // Calculate the Merkle tree hashes
    computeHashArray(hasharray, numLeafNodes);

    // Print the internal Merkle tree node hashes
    printf("\nInternal Merkle Tree Node Hashes:\n");
    for (i = 0; i < numLeafNodes-1; i++) {
        printf("Node %d: %s\n", i + numLeafNodes, hasharray[i].hash);
    }

    // Free allocated memory
    free(hasharray);

    return 0;
}
