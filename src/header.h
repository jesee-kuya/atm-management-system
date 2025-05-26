#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>



struct Date {
    int month, day, year;
};

struct Record {
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User {
    int id;
    char name[50];
    char password[50];
};

// Authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
int getUser(struct User *u);
void ensureUsersFileExists();

// System functions
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void updateAccount(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwner(struct User u);
void ensureRecordsFileExists();
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);
int accountNumberExists(FILE *ptr, int accountNbr);
double simple_interest(double principal, double rate, double time);

// Validation functions
int isValidDate(int month, int day, int year);
int isValidPhone(const char *phone);
int isValidAmount(double amount);
int isValidAccountType(const char *type);
int isValidName(const char *name);
int isValidCountry(const char *country);
void trimNewline(char *str);
int isValidPassword(const char *password);
void removeWhitespace(char *str);