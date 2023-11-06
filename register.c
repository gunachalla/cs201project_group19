#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct politicalpartieslist {
    char partyname[100];
    char symbol[100];
    char contestent[100];
};

typedef struct politicalpartieslist partyregister;

int main() {
    partyregister *partylist = NULL;
    int Noofparties = 0;
    char choice;

    FILE *file = fopen("party_data.txt", "w+"); // Open a file for writing

    if (file == NULL) {
        printf("Failed to open the file for writing. Exiting...\n");
        return 1;
    }

    printf("Register the parties contesting for the election\n");
    printf("Press 'a' or 'A' to register a party\n");
    printf("Enter 'q' to stop\n");

    while (1) {
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        if (choice == 'q' || choice == 'Q') {
            break;
        } else if (choice == 'a' || choice == 'A') {
            partylist = (partyregister *)realloc(partylist, sizeof(partyregister) * (Noofparties + 1));

            if (partylist == NULL) {
                printf("Memory allocation failed. Exiting...\n");
                exit(1);
            }

            printf("Enter the name of the party: ");
            scanf("%s", partylist[Noofparties].partyname);
            printf("Enter the symbol of the party: ");
            scanf("%s", partylist[Noofparties].symbol);
            printf("Enter the contestant of the party: ");
            scanf("%s", partylist[Noofparties].contestent);

            // Write the party data to the file
            fprintf(file, "Party Name: %s\n", partylist[Noofparties].partyname);
            fprintf(file, "Party Symbol: %s\n", partylist[Noofparties].symbol);
            fprintf(file, "Contestant: %s\n", partylist[Noofparties].contestent);
            fprintf(file, "\n");

            Noofparties++;
        } else {
            printf("Invalid choice. Please enter 'a' or 'A' to register a party or 'q' to stop.\n");
        }
    }

    // Close the file
    fclose(file);

    // Print the registered parties
    printf("\nRegistered Parties:\n");
    for (int i = 0; i < Noofparties; i++) {
        printf("Party Name: %s\n", partylist[i].partyname);
        printf("Party Symbol: %s\n", partylist[i].symbol);
        printf("Contestant: %s\n", partylist[i].contestent);
        printf("\n");
    }

    free(partylist);  // Free the allocated memory

    return 0;
}
