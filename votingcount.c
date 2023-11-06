#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SHA256_HASH_SIZE 64

// Function to check if a string is a valid SHA-256 hash
bool isSHA256Hash(const char *str) {
    // Check if the string length is valid
    if (strlen(str) != SHA256_HASH_SIZE) {
        return false;
    }

    // Check if all characters are hexadecimal digits
    for (int i = 0; i < SHA256_HASH_SIZE; i++) {
        if ((str[i] < '0' || str[i] > '9') && (str[i] < 'a' || str[i] > 'f')) {
            return false;
        }
    }

    return true;
}

// Function to count the number of votes for a party in a file
int countVotesInFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file %s for reading.\n", filename);
        return -1;  // Return -1 to indicate an error
    }

    int count = 0;
    char buffer[SHA256_HASH_SIZE + 1];  // +1 for the null terminator

    while (fscanf(file, "%64s", buffer) == 1) {
        if (isSHA256Hash(buffer)) {
            count++;
        }
    }

    fclose(file);
    return count;
}

int main() {
    const char *partyFiles[] = {"party1.txt", "party2.txt", "party3.txt", "nota.txt"};
    const int numParties = sizeof(partyFiles) / sizeof(partyFiles[0]);

    int partyVotes[numParties];
    
    // Count the votes for each party
    for (int i = 0; i < numParties; i++) {
        partyVotes[i] = countVotesInFile(partyFiles[i]);

        if (partyVotes[i] == -1) {
            return 1;  // Error opening a file
        }
    }

    // Find the party with the most votes
    int maxVotes = partyVotes[0];
    int winningParty = 0;

    for (int i = 1; i < numParties; i++) {
        if (partyVotes[i] > maxVotes) {
            maxVotes = partyVotes[i];
            winningParty = i;
        }
    }

    if (winningParty == numParties - 1) {
        printf("NOTA (None of the Above) wins with %d votes.\n", maxVotes);
    } else {
        printf("Party %d wins with %d votes.\n", winningParty + 1, maxVotes);
    }

    return 0;
}
