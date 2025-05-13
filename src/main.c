#include "header.h"

void mainMenu(struct User u) {
    int option;
    system("clear");
    printf("\n\n\t\t\t\tв•”в•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•—");
    printf("\n\t\t\t\tв•‘           ATM                                                                      в•‘");
    printf("\n\t\t\t\tв•љв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ќ\n");
    printf("\n\t\t[1] Create a new account\n");
    printf("\t\t[2] Update account\n");
    printf("\t\t[3] Check account details\n");
    printf("\t\t[4] List all accounts\n");
    printf("\t\t[5] Make transaction\n");
    printf("\t\t[6] Delete account\n");
    printf("\t\t[7] Transfer ownership\n");
    printf("\t\t[8] Exit\n");
    printf("\n\t\tEnter choice: ");
    
    while (scanf("%d", &option) != 1 || option < 1 || option > 8) {
        printf("\t\tInvalid input! Enter 1-8: ");
        while (getchar() != '\n');
    }

    switch (option) {
        case 1: createNewAcc(u); break;
        case 2: updateAccount(u); break;
        case 3: checkAccountDetails(u); break;
        case 4: checkAllAccounts(u); break;
        case 5: makeTransaction(u); break;
        case 6: removeAccount(u); break;
        case 7: transferOwner(u); break;
        case 8: exit(0);
    }
}

void initMenu(struct User *u) {
    int option;
    char password[50];
    char input[100];

    while (1) {
        system("clear");
        printf("\n\n\t\t\t\tв•”в•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•—");
        printf("\n\t\t\t\tв•‘                                    Welcome to ATM                                  в•‘");
        printf("\n\t\t\t\tв•љв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ќ\n");
        printf("\n\t\t[1] Login\n\t\t[2] Register\n\t\t[3] Exit\n\t\tChoice: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // remove newline

        if (strlen(input) == 0) {
            continue;  // just Enter was pressed, redisplay menu
        }

        int valid = 1;
        for (int i = 0; i < strlen(input); i++) {
            if (!isdigit(input[i])) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            option = atoi(input);
            if (option >= 1 && option <= 3) {
                break;  // valid choice
            }
        }

        printf("\t\tInvalid input! Enter a number between 1 and 3.\n");
        sleep(1);  // slight delay before showing menu again
    }

    switch (option) {
        case 1:
            loginMenu(u->name, password);
            if (getUser(u) && strcmp(password, u->password) == 0) {
                printf("\n\t\t✓ Login successful!\n");
                sleep(1);
            } else {
                printf("\n\t\t✗ Invalid credentials!\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 2:
            registerMenu(u->name, u->password);
            break;
        case 3:
            exit(0);
    }
}

int main() {
    struct User u = {0};
    initMenu(&u);
    mainMenu(u);
    return 0;
}