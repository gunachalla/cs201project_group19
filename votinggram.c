#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SHA256.h"

struct politicalpartieslist {
    char partyname[100];
    char symbol[100];
    char contestant[100];
};

typedef struct politicalpartieslist partyregister;

// Function to convert a string to uint8_t array
void stringToUint8(const char *str, uint8_t *uint8Array) {
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len; i++) {
        uint8Array[i] = (uint8_t)str[i];
    }
}


int hasVoterAlreadyVoted(const char *uniqueID) {
    FILE *votedFile = fopen("voted.txt", "r");
    if (votedFile == NULL) {
        printf("Error: Could not open the voted file.\n");
        exit(1);
    }

    char line[10];
    while (fgets(line, sizeof(line), votedFile) != NULL) {
        if (strcmp(line, uniqueID) == 0) {
            fclose(votedFile);
            return 1; // Voter has already voted
        }
    }

    fclose(votedFile);
    return 0; // Voter has not voted yet
}

// Function to mark a voter as voted
void markVoterAsVoted(const char *uniqueID) {
    FILE *votedFile = fopen("voted.txt", "a");
    if (votedFile == NULL) {
        printf("Error: Could not open the voted file.\n");
        exit(1);
    }
    fprintf(votedFile, "\n%s\n", uniqueID);
    fclose(votedFile);
}

int main() {
    FILE *votersFile;
    char line[200];
    char uniqueID[7];
    votersFile = fopen("voterslist.txt", "r");

    if (votersFile == NULL) {
        printf("Error: Could not open the voter list file.\n");
        return 1;
    }

    printf("Enter the unique ID to search for: ");
    scanf("%6s", uniqueID);
    int found = 0;

    while (fgets(line, sizeof(line), votersFile) != NULL) {
        if (strstr(line, uniqueID) != NULL) {
            found = 1;
            printf("Voter Information:\n");
            printf("%s", line);

            if (hasVoterAlreadyVoted(uniqueID)) {
                printf("This voter has already cast their vote.\n");
            } 
            else {
                markVoterAsVoted(uniqueID); // Mark the voter as voted
                FILE * voted;
                voted=fopen("voted.txt","a");
                fprintf(voted,"\n%s",uniqueID);
                // Voting logic: allow the voter to cast their vote
                int choice;
                printf("Menu:\n");
                printf("1. for party 1\n");
                printf("2. for party 2\n");
                printf("3. for party 3\n");
                printf("4. for NOTA\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                if (choice >= 1 && choice <= 4) {
                    uint8_t uint8Array[strlen(line)];
                    stringToUint8(line, uint8Array);

                    uint8_t str[65];
                    sha256(uint8Array, strlen((char *)uint8Array), str);
                    char hexDigest[2 * SHA256_DIGEST_SIZE + 1]; // +1 for the null terminator
                    for (size_t i = 0; i < SHA256_DIGEST_SIZE; i++) {
                        snprintf(&hexDigest[i * 2], 3, "%02x", str[i]);
                    }
                    hexDigest[2 * SHA256_DIGEST_SIZE] = '\0';

                    char filename[10];
                    if (choice == 4) {
                        strcpy(filename, "NOTA.txt");
                    } 
                     if (choice == 1) {
                        strcpy(filename, "party1.txt");
                    } 
                     if (choice == 2) {
                        strcpy(filename, "party2.txt");
                    } 
                     if (choice == 3) {
                        strcpy(filename, "party3.txt");
                    } 

                    FILE *file = fopen(filename, "a");
                    if (file == NULL) {
                        printf("Error opening the file %s.\n", filename);
                        return 1;
                    }

                    // Write the string to the file
                    if (fprintf(file, "\n%s", hexDigest) < 0) {
                        printf("Error writing to the file %s.\n", filename);
                        fclose(file);
                        return 1;
                    }

                    printf("Vote cast successfully!\n");
                    fclose(file);
                } else {
                    printf("Invalid choice. Please try again.\n");
                }
            }
            break;
        }
    }

    fclose(votersFile);

    if (!found) {
        printf("Voter with ID %s not found.\n", uniqueID);
    }

    return 0;
}
