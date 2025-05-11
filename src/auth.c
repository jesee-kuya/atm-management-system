#include <termios.h>
#include <unistd.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50]) {
    struct termios oldt, newt;
    system("clear");
    printf("\n\n\t\t\t\t╔════════════════════════════╗");
    printf("\n\t\t\t\t║       User Login           ║");
    printf("\n\t\t\t\t╚════════════════════════════╝\n");
    printf("\n\t\t\t\tEnter username: ");
    scanf("%49s", a);

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
    struct User newUser = {0};
    int maxId = 0;
    FILE *fp = fopen(USERS, "r");

    // Find max ID
    if (fp) {
        int currentId;
        while (fscanf(fp, "%d %*s %*s", &currentId) != EOF) {
            if (currentId > maxId) maxId = currentId;
        }
        fclose(fp);
    }
    newUser.id = maxId + 1;

    system("clear");
    printf("\n\n\t\t\t\t╔════════════════════════════╗");
    printf("\n\t\t\t\t║       Registration        ║");
    printf("\n\t\t\t\t╚════════════════════════════╝\n");
    printf("\n\t\t\t\tEnter username: ");
    scanf("%49s", newUser.name);
    printf("\t\t\t\tEnter password: ");
    scanf("%49s", newUser.password);

    fp = fopen(USERS, "a");
    if (!fp) {
        perror("Failed to open users file");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);

    strcpy(a, newUser.name);
    strcpy(pass, newUser.password);
    printf("\n\t\t\t\t✅ Registration successful!\n");
    sleep(1);
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