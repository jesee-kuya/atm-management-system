#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
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

void saveAccountToFile(FILE *ptr, struct User *u, struct Record *r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
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


void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, &u, &r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
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
    int accNumber;
    printf("Enter account number to update: ");
    scanf("%d", &accNumber);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("temp.txt", "w");
    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            printf("Enter new country (current: %s): ", r.country);
            scanf("%99s", r.country);
            printf("Enter new phone (current: %d): ", r.phone);
            scanf("%d", &r.phone);
        }
        saveAccountToFile(tmp, &(struct User){.id = r.userId}, &r);
    }
    fclose(pf);
    fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Account updated.\n");
    } else {
        remove("temp.txt");
        printf("Account not found.\n");
    }
    stayOrReturn(!found, updateAccount, u);
}

void checkAccountDetails(struct User u) {
    int accNumber;
    printf("Enter account number: ");
    scanf("%d", &accNumber);

    FILE *pf = fopen(RECORDS, "r");
    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            printf("\nAccount Number: %d\nCountry: %s\nPhone: %d\nAmount: $%.2f\nType: %s\n",
                   r.accountNbr, r.country, r.phone, r.amount, r.accountType);
            break;
        }
    }
    fclose(pf);
    stayOrReturn(!found, checkAccountDetails, u);
}

void makeTransaction(struct User u) {
    int choice;
    double amount;
    int accNumber;
    printf("Enter account number: ");
    scanf("%d", &accNumber);
    printf("1. Deposit\n2. Withdraw\nChoose: ");
    scanf("%d", &choice);
    printf("Enter amount: ");
    scanf("%lf", &amount);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("temp.txt", "w");
    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            found = 1;
            if (choice == 1) r.amount += amount;
            else if (r.amount >= amount) r.amount -= amount;
            else {
                printf("Insufficient funds.\n");
                fclose(pf); fclose(tmp); remove("temp.txt");
                stayOrReturn(0, makeTransaction, u);
                return;
            }
        }
        saveAccountToFile(tmp, &u, &r);
    }
    fclose(pf); fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Transaction successful.\n");
    } else {
        remove("temp.txt");
        printf("Account not found.\n");
    }
    stayOrReturn(!found, makeTransaction, u);
}

void removeAccount(struct User u) {
    int accNumber;
    printf("Enter account number to delete: ");
    scanf("%d", &accNumber);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("temp.txt", "w");
    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) found = 1;
        else saveAccountToFile(tmp, &u, &r);
    }
    fclose(pf); fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Account removed.\n");
    } else {
        remove("temp.txt");
        printf("Account not found.\n");
    }
    stayOrReturn(!found, removeAccount, u);
}

void transferOwner(struct User u) {
    int accNumber;
    char newUser[50];
    printf("Enter account number: ");
    scanf("%d", &accNumber);
    printf("Enter new owner's username: ");
    scanf("%49s", newUser);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("temp.txt", "w");
    struct Record r;
    char user[50];
    int found = 0;

    while (getAccountFromFile(pf, user, &r)) {
        if (strcmp(user, u.name) == 0 && r.accountNbr == accNumber) {
            strcpy(user, newUser);
            found = 1;
        }
        saveAccountToFile(tmp, &(struct User){.name = user}, &r);
    }
    fclose(pf); fclose(tmp);

    if (found) {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("Ownership transferred.\n");
    } else {
        remove("temp.txt");
        printf("Account not found.\n");
    }
    stayOrReturn(!found, transferOwner, u);
}