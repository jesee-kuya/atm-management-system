#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50]) {
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%49s", a);

    // Disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%49s", pass);

    // Restore terminal
    tcsetattr(fileno(stdin), TCSANOW, &oflags);
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
    struct User newUser = {0};
    FILE *fp = fopen(USERS, "r");
    
    // Find max ID
    while (fp && fscanf(fp, "%d %*s %*s", &newUser.id) != EOF) {
        if (newUser.id > newUser.id) newUser.id = newUser.id;
    }
    newUser.id++;
    if (fp) fclose(fp);

    system("clear");
    printf("\n\n\t\t\t\tEnter username: ");
    scanf("%49s", newUser.name);
    printf("\n\t\t\t\tEnter password: ");
    scanf("%49s", newUser.password);

    fp = fopen(USERS, "a");
    if (!fp) {
        perror("Failed to open users file");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);

    strcpy(a, newUser.name);
    strcpy(pass, newUser.password);
    printf("\n\t\t\t\tRegistration successful!\n");
}

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}