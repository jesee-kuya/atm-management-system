#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
    return fscanf(ptr, "%d %d %49s %d %d/%d/%d %49s %d %lf %9s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

int accountNumberExists(FILE *ptr, int accountNbr) {
    struct Record temp;
    char tempName[50];

    while (fscanf(ptr, "%d %d %49s %d %d/%d/%d %49s %d %lf %9s",
                  &temp.id,
                  &temp.userId,
                  tempName,
                  &temp.accountNbr,
                  &temp.deposit.month,
                  &temp.deposit.day,
                  &temp.deposit.year,
                  temp.country,
                  &temp.phone,
                  &temp.amount,
                  temp.accountType) != EOF) {

        if (temp.accountNbr == accountNbr) {
            return 1; 
        }
    }


    return 0; 
}

void saveAccountToFile(FILE *ptr, struct User *u, struct Record *r) {
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r->id,
            u->id,
            u->name,
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
    char input[10];
    int option;
    if (notGood == 0) {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        fgets(input, sizeof(input), stdin);
        option = atoi(input);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    } else {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        fgets(input, sizeof(input), stdin);
        option = atoi(input);
    }
    if (option == 1) {
        system("clear");
        mainMenu(u);
    } else {
        system("clear");
        exit(1);
    }
}

void success(struct User u) {
    char input[10];
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    fgets(input, sizeof(input), stdin);
    option = atoi(input);
    system("clear");
    if (option == 1) {
        mainMenu(u);
    } else if (option == 0) {
        exit(1);
    } else {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u) {
    ensureRecordsFileExists();
    struct Record r, cr;
    char userName[50];

    // Open or create RECORDS file
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }

    // Get the max ID from existing records
    rewind(pf);
    int maxId = 0;
    while (getAccountFromFile(pf, userName, &cr)) {
        if (cr.id > maxId) {
            maxId = cr.id;
        }
    }
    r.id = maxId + 1;

    int validAccountCreated = 0;
    while (!validAccountCreated) {
        system("clear");
        printf("\t\t\t===== New record =====\n");

        // --- DATE INPUT ---
        int validDate = 0;
        char dateInput[20];

       do {
            system("clear");
            printf("\t\t\t===== New record =====\n");
            printf("\nEnter today's date (mm/dd/yyyy): ");
            if (!fgets(dateInput, sizeof(dateInput), stdin)) {
                printf("Input error!\n");
                clearerr(stdin);  // Clear error state if stdin is broken
                continue;
            }

            // Check if input was truncated (no newline found)
            if (strchr(dateInput, '\n') == NULL) {
                // Flush excess characters from input buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Invalid date format! Use mm/dd/yyyy with no extra characters.\n");
                sleep(2);
                continue;
            }

            removeWhitespace(dateInput);

            int n;  // To track characters parsed by sscanf
            if (sscanf(dateInput, "%d/%d/%d%n", &r.deposit.month, &r.deposit.day, &r.deposit.year, &n) != 3) {
                printf("Invalid date format! Use mm/dd/yyyy with no extra characters.\n");
                sleep(2);
                continue;
            }

            // Check if there are leftover characters after the date
            if (dateInput[n] != '\0') {
                printf("Invalid date format! Extra characters detected.\n");
                sleep(2);
                continue;
            }

            validDate = isValidDate(r.deposit.month, r.deposit.day, r.deposit.year);
            if (!validDate) {
                printf("Invalid date values!\n");
                sleep(2);
                continue;
            }
        } while (!validDate);

        int accounErr = 0;
        int duplicateFound = 0;
        // --- ACCOUNT NUMBER ---
        char accInput[20];
        while (1) {
            system("clear");
            printf("\t\t\t===== New record =====\n");
            printf("\nEnter the account number: ");
            fgets(accInput, sizeof(accInput), stdin);
            accInput[strcspn(accInput, "\n")] = 0;
            if (strlen(accInput) == 0 || strspn(accInput, "0123456789") != strlen(accInput)) {
                printf("Invalid account number! Must be digits only.\n");
                sleep(2);
                continue;
            }
            r.accountNbr = atoi(accInput);
            if (r.accountNbr <= 0) {
                printf("Account number must be positive.\n");
                sleep(2);
                continue;
            }

            rewind(pf);
            if (accountNumberExists(pf, r.accountNbr)) {
                printf("Account number already exists! Please try another.\n");
                sleep(2);
                continue;
            }
           
            rewind(pf);
            while (getAccountFromFile(pf, userName, &cr)) {
                if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
                    duplicateFound = 1;
                    break;
                }
            }

            break;
        }

         if (duplicateFound) {
            printf("✖ This account already exists for this user. Please try another account number.\n");
            sleep(2);
            continue;
        }


        // --- COUNTRY ---
        do {
            system("clear");
            printf("\t\t\t===== New record =====\n");
            printf("\nEnter the country: ");
            fgets(r.country, sizeof(r.country), stdin);
            r.country[strcspn(r.country, "\n")] = 0;
            if (!isValidCountry(r.country) || r.country[0] == '\0') {
                printf("Invalid country name! Use only letters, hyphens, apostrophes, and periods (2–50 chars).\n");
                sleep(2);
                continue;
            }

        } while (!isValidCountry(r.country));

        // --- PHONE ---
        char phoneStr[20];
        do {
            system("clear");
            printf("\t\t\t===== New record =====\n");
            printf("\nEnter the phone number: ");
            fgets(phoneStr, sizeof(phoneStr), stdin);
            phoneStr[strcspn(phoneStr, "\n")] = 0;
            if (!isValidPhone(phoneStr)) {
                printf("Invalid phone number! Use digits only (8–15 characters).\n");
                sleep(2);
                continue;
            }
        } while (!isValidPhone(phoneStr));
        r.phone = atoi(phoneStr);

        // --- AMOUNT ---
        char amountInput[30];
        while (1) {
            system("clear");
            printf("\t\t\t===== New record =====\n");   
            printf("\nEnter amount to deposit: $");
            fgets(amountInput, sizeof(amountInput), stdin);
            if (sscanf(amountInput, "%lf", &r.amount) != 1 || r.amount <= 0) {
                printf("Invalid amount! Enter a positive number.\n");
                sleep(2);
                continue;
            }
            break;
        }

        // --- ACCOUNT TYPE ---
       do {
            system("clear");
            printf("\t\t\t===== New record =====\n");
            printf("\nChoose the type of account:\n"
                "\t-> saving\n\t-> current\n\t-> fixed01 (1 yr)\n"
                "\t-> fixed02 (2 yrs)\n\t-> fixed03 (3 yrs)\n"
                "\tEnter your choice: ");
            
            // Read input
            if (!fgets(r.accountType, sizeof(r.accountType), stdin)) {
                printf("Input error!\n");
                clearerr(stdin);  // Clear error state if stdin fails
                sleep(2);
                continue;
            }

            // Check for input truncation (no newline found)
            if (strchr(r.accountType, '\n') == NULL) {
                // Flush excess characters from input buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Invalid account type! Choose from: saving, current, fixed01, fixed02, fixed03\n");
                sleep(2);
                continue;
            }

            // Remove ALL whitespace (including newlines/tabs)
            removeWhitespace(r.accountType);

            // Validate cleaned input
            if (!isValidAccountType(r.accountType)) {
                printf("Invalid account type! Choose from: saving, current, fixed01, fixed02, fixed03\n");
                sleep(2);
                continue;
            }
        } while (!isValidAccountType(r.accountType));

        r.userId = u.id;

        // --- SAVE RECORD ---
        saveAccountToFile(pf, &u, &r);
        validAccountCreated = 1;
    }

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u) {
    ensureRecordsFileExists();
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0) {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.month,
                   r.deposit.day,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccount(struct User u) {
    ensureRecordsFileExists();
    system("clear");
    printf("\t\t\t===== Update account =====\n");

    char accInput[20];
    int accNumber;

    printf("Enter account number to update: ");
    fgets(accInput, sizeof(accInput), stdin);
    accNumber = atoi(accInput);

    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }

    FILE *tmp = fopen("temp.txt", "w");
    if (!tmp) {
        perror("Failed to open temporary file");
        fclose(pf);
        exit(EXIT_FAILURE);
    }

    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;

            char country[100];
            strcpy(country, r.country);

            do {
                system("clear");
                printf("\t\t\t===== Update account =====\n");
                printf("Enter new country (current: %s): ", country);
                fgets(r.country, sizeof(r.country), stdin);
                r.country[strcspn(r.country, "\n")] = 0;
                if (!isValidCountry(r.country) || r.country[0] == '\0') {
                    printf("Invalid country name! Use only letters, hyphens, apostrophes, and periods (2–50 chars).\n");
                    sleep(2);
                    continue;
                }

            } while (!isValidCountry(r.country));


            // Update phone with validation
            char phoneStr[20];
            int phone = r.phone;
            
            do {
                system("clear");
                printf("\t\t\t===== Update account =====\n");
                printf("Enter new phone (current: %d): ", phone);
                fgets(phoneStr, sizeof(phoneStr), stdin);
                phoneStr[strcspn(phoneStr, "\n")] = 0;

                if (!isValidPhone(phoneStr)) {
                    printf("You entered: %s\n", phoneStr);
                    printf("Invalid phone number! Use digits only (8–15 characters).\n");
                    sleep(2);
                    continue;
                }
            } while (!isValidPhone(phoneStr));
            r.phone = atoi(phoneStr);
        }

        // Save record with correct username
        struct User tempUser = {.id = r.userId};
        strcpy(tempUser.name, user);
        saveAccountToFile(tmp, &tempUser, &r);
    }

    fclose(pf);
    fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Account updated.\n");
    } else {
        remove("temp.txt");
        printf("✖ Account not found.\n");
    }

    stayOrReturn(1, updateAccount, u);
}

void checkAccountDetails(struct User u) {
    ensureRecordsFileExists();
    system("clear");
    printf("\t\t\t===== Check Account Details =====\n");

    char accInput[20];
    int accNumber;

    printf("Enter account number: ");
    fgets(accInput, sizeof(accInput), stdin);
    accInput[strcspn(accInput, "\n")] = 0;
    accNumber = atoi(accInput);

    if (accNumber <= 0) {
        printf("✖ Invalid account number!\n");
        stayOrReturn(1, checkAccountDetails, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }

    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            printf("\nAccount Details:\n");
            printf("─────────────────────────────\n");
            printf(" Account Number : %d\n", r.accountNbr);
            printf(" Country        : %s\n", r.country);
            printf(" Phone          : %d\n", r.phone);
            printf(" Amount         : $%.2f\n", r.amount);
            printf(" Account Type   : %s\n", r.accountType);
            printf(" Date Created   : %02d/%02d/%d\n",
                   r.deposit.month, r.deposit.day, r.deposit.year);
            printf("─────────────────────────────\n");

            if (strcmp("saving", r.accountType) == 1) {
                double amount = simple_interest(r.amount, 7.0, 1.0);
                printf("You will get $%.2f as interest on day %02d of every month\n", amount, r.deposit.day);
            } else if (strcmp("fixed01", r.accountType) == 1) {
                double amount = simple_interest(r.amount, 4.0, 1.0);
                printf("You will get $%.2f as interest on day %02d of every month\n", amount, r.deposit.day);
            } else if (strcmp("fixed02", r.accountType) == 1) {
                double amount = simple_interest(r.amount, 5.0, 2.0);
                printf("You will get $%.2f as interest on day %02d of every month\n", amount, r.deposit.day);
            } else if (strcmp("fixed03", r.accountType) == 1) {
                double amount = simple_interest(r.amount, 8.0, 3.0);
                printf("You will get $%.2f as interest on day %02d of every month\n", amount, r.deposit.day);
            } else {
                printf("You will not get interests because the account is of type current");
            }

            break;
        }
        
    }

    fclose(pf);

    if (!found) {
        printf("✖ Account not found for this user.\n");
    }

    stayOrReturn(1, checkAccountDetails, u);
}

void makeTransaction(struct User u) {
    ensureRecordsFileExists();
    system("clear");
    printf("\t\t\t===== Make a transaction =====\n");

    ensureRecordsFileExists();
    char accInput[20], choiceInput[10], amountInput[20];
    int accNumber, choice;
    double amount;

    printf("Enter account number: ");
    fgets(accInput, sizeof(accInput), stdin);
    accNumber = atoi(accInput);

    printf("1. Deposit\n2. Withdraw\nChoose: ");
    fgets(choiceInput, sizeof(choiceInput), stdin);
    choice = atoi(choiceInput);

    do {
        printf("Enter amount: ");
        fgets(amountInput, sizeof(amountInput), stdin);
        amount = atof(amountInput);

    } while(!isValidAmount(amount));

    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }
    FILE *tmp = fopen("temp.txt", "w");
    if (!tmp) {
        perror("Failed to open temporary file");
        fclose(pf);
        exit(EXIT_FAILURE);
    }

    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            if (choice == 1) {
                r.amount += amount;
            } else if (choice == 2) {
                if (r.amount >= amount) {
                    r.amount -= amount;
                } else {
                    printf("Insufficient funds.\n");
                    fclose(pf);
                    fclose(tmp);
                    remove("temp.txt");
                    stayOrReturn(0, makeTransaction, u);
                    return;
                }
            }
        }

        if (strcmp("fixed01", r.accountType) == 1|| strcmp("fixed02", r.accountType) == 1 || strcmp("fixed03", r.accountType) == 1 ) {
            printf("✖ Cannot make transactions on fixed accounts.\n");
            fclose(pf);
            fclose(tmp);
            remove("temp.txt");
            stayOrReturn(1, makeTransaction, u);
            return;
        }
        saveAccountToFile(tmp, &u, &r);
    }

    fclose(pf);
    fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Transaction successful.\n");
    } else {
        remove("temp.txt");
        printf("Account not found.\n");
    }
    stayOrReturn(1,  makeTransaction, u);
}

void removeAccount(struct User u) {
    ensureRecordsFileExists();
    system("clear");
    printf("\t\t\t===== Remove account =====\n");

    char accInput[20];
    int accNumber;

    printf("Enter account number: ");
    if (!fgets(accInput, sizeof(accInput), stdin)) {
        printf("Error reading input.\n");
        sleep(2);
        return;
    }

    // Remove newline
    accInput[strcspn(accInput, "\n")] = 0;

    // Validate: non-empty and all digits
    if (strlen(accInput) == 0 || strspn(accInput, "0123456789") != strlen(accInput)) {
        printf("Invalid account number! Must be digits only.\n");
        sleep(2);
        return;
    }

    accNumber = atoi(accInput);

    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }

    FILE *tmp = fopen("temp.txt", "w");
    if (!tmp) {
        perror("Failed to open temporary file");
        fclose(pf);
        exit(EXIT_FAILURE);
    }

    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            // Do not write this account to temp file
        } else {
            saveAccountToFile(tmp, &u, &r);
        }
    }

    fclose(pf);
    fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Account removed.\n");
    } else {
        remove("temp.txt");
        printf("Account not found.\n");
    }

    sleep(2);  // Optional delay to show message
    stayOrReturn(1, removeAccount, u);
}

void transferOwner(struct User u) {
    ensureRecordsFileExists();
    system("clear");
    printf("\t\t\t===== Transfer ownership =====\n");

    int accNumber;
    char newUser[50];

    printf("Enter account number: ");
    scanf("%d", &accNumber);
    getchar(); 

    do {
        printf("Enter new owner's username: ");
        fgets(newUser, sizeof(newUser), stdin);
        newUser[strcspn(newUser, "\n")] = 0;

        if (strcmp(newUser, u.name) == 0) {
            printf("Cannot transfer to yourself!\n");
        } else if (!isValidName(newUser)) {
            printf("Invalid username format!\n");
        }
    } while (strcmp(newUser, u.name) == 0 || !isValidName(newUser));

    // Check if new user exists
    struct User newOwner = {0};
    strcpy(newOwner.name, newUser);
    if (!getUser(&newOwner)) {
        printf("✖ New user does not exist!\n");
        stayOrReturn(0, transferOwner, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        perror("Failed to open records file");
        exit(EXIT_FAILURE);
    }

    FILE *tmp = fopen("temp.txt", "w");
    if (!tmp) {
        perror("Failed to open temporary file");
        fclose(pf);
        exit(EXIT_FAILURE);
    }

    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            r.userId = newOwner.id;
            strcpy(user, newUser);
        }

    
        struct User tempUser;
        tempUser.id = r.userId;
        strcpy(tempUser.name, user);
        saveAccountToFile(tmp, &tempUser, &r);
    }

    fclose(pf);
    fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Ownership transferred.\n");
    } else {
        remove("temp.txt");
        printf("✖ Account not found.\n");
    }

    stayOrReturn(1, transferOwner, u);
}

void ensureRecordsFileExists() {
    struct stat st = {0};
    if (stat("data", &st) == -1) {
        if (mkdir("data", 0700) != 0) {
            perror("Failed to create 'data' directory");
            exit(EXIT_FAILURE);
        }
    }

    FILE *fp = fopen(RECORDS, "a");
    if (!fp) {
        perror("Failed to create 'records.txt' file");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
}

double simple_interest(double principal, double rate, double time) {
    return (principal * rate * time) / 100.0;
}