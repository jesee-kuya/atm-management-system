#include <termios.h>
#include <unistd.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50]) {
    ensureUsersFileExists();
    struct termios oldt, newt;
    system("clear");
    printf("\n\n\t\t\t\tв•”в•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•—");
    printf("\n\t\t\t\tв•‘                                  User Login                                        в•‘");
    printf("\n\t\t\t\tв•љв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ђв•ќ\n");
    printf("\n\t\t\t\tEnter username: ");
    do {
        scanf("%49s", a);
        if (!isValidName(a)) {
            printf("Invalid username! Use only alphanumeric, _ and - (3-49 chars): ");
        }
    } while (!isValidName(a));

    // Disable echo for password
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("\t\t\t\tEnter password: ");
    scanf("%49s", pass);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}

int getUser(struct User *u) {
    FILE *fp = fopen(USERS, "r");
    if (!fp) return 0;

    struct User temp;
    while (fscanf(fp, "%d %49s %49s", &temp.id, temp.name, temp.password) != EOF) {
        if (strcmp(temp.name, u->name) == 0) {
            *u = temp;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void registerMenu(char a[50], char pass[50]) {
    ensureUsersFileExists();

    struct User newUser = {0};
    int maxId = 0;
    struct User temp;

    // Determine the next available ID by scanning existing users
    FILE *fp = fopen(USERS, "r");
    if (!fp) {
        perror("Failed to open users file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%d %49s %49s", &temp.id, temp.name, temp.password) == 3) {
        if (temp.id > maxId) {
            maxId = temp.id;
        }
    }
    fclose(fp);

    newUser.id = maxId + 1;

    system("clear");
    printf("\n\n\t\t\t\tRegistration\n");

    // Username input with validation and existence check
    int usernameTaken;
    char buffer[100];

    do {
        usernameTaken = 0;
        printf("\n\t\t\t\tEnter username: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\t\t\t\tError reading input!\n");
            continue;
        }

        trimNewline(buffer);

        if (!isValidName(buffer)) {
            printf("\t\t\t\tInvalid username! Use only alphanumeric, _ and - (3–49 chars)\n");
            continue;
        }

        strncpy(newUser.name, buffer, sizeof(newUser.name));
        newUser.name[sizeof(newUser.name) - 1] = '\0';

        fp = fopen(USERS, "r");
        if (!fp) {
            perror("Failed to open users file");
            exit(EXIT_FAILURE);
        }

        while (fscanf(fp, "%d %49s %49s", &temp.id, temp.name, temp.password) == 3) {
            if (strcmp(temp.name, newUser.name) == 0) {
                usernameTaken = 1;
                printf("\t\t\t\tUsername already exists! Try another one.\n");
                break;
            }
        }
        fclose(fp);
    } while (!isValidName(newUser.name) || usernameTaken);

    // Password confirmation loop
    char confirmPass[50];
    char password[50];
    do {
        printf("\t\t\t\tEnter password: ");
        if (!fgets(password, sizeof(password), stdin)) {
            printf("\t\t\t\tError reading input!\n");
            continue;
        }
        trimNewline(password);

        if (!isValidPassword(password)) {
            printf("\t\t\t\tInvalid password! Use only alphanumeric, _ and - (3–49 chars)\n");
            continue;
        }
        strncpy(newUser.password, password, sizeof(newUser.password));
        newUser.password[sizeof(newUser.password) - 1] = '\0';

        printf("\t\t\t\tConfirm password: ");
        if (!fgets(confirmPass, sizeof(confirmPass), stdin)) {
            printf("\t\t\t\tError reading input!\n");
            continue;
        }
        trimNewline(confirmPass);

        if (strcmp(newUser.password, confirmPass) != 0) {
            printf("\t\t\t\tPasswords don't match! Try again.\n");
        }
    } while (strcmp(newUser.password, confirmPass) != 0);

    // Save new user
    fp = fopen(USERS, "a");
    if (!fp) {
        perror("Failed to open users file");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);

    strcpy(a, newUser.name);
    strcpy(pass, newUser.password);

    printf("\n\t\t\t\tRegistration successful!\n");
    sleep(1);
}

void ensureUsersFileExists() {
    struct stat st = {0};
    if (stat("data", &st) == -1) {
        if (mkdir("data", 0700) != 0) {
            perror("Failed to create 'data' directory");
            exit(EXIT_FAILURE);
        }
    }

    FILE *fp = fopen(USERS, "a");
    if (!fp) {
        perror("Failed to create 'users.txt' file");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
}
