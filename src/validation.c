#include "header.h"

int isValidDate(int month, int day, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    // April, June, September, November
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return 0;
    
    // February
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) return 0;
        } else {
            if (day > 28) return 0;
        }
    }
    return 1;
}

int isValidPhone(const char *phone) {
    if (strlen(phone) < 8 || strlen(phone) > 15) return 0;
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+') return 0;
    }
    return 1;
}

int isValidAmount(double amount) {
    return amount > 0;
}

int isValidAccountType(const char *type) {
    const char *validTypes[] = {"saving", "current", "fixed01", "fixed02", "fixed03"};
    for (int i = 0; i < 5; i++) {
        if (strcasecmp(type, validTypes[i]) == 0) return 1;
    }
    return 0;
}

int isValidName(const char *name) {
    size_t len = strlen(name);
    if (len < 3 || len > 49) {
        return 0;
    }
    for (int i = 0; name[i]; i++) {
        if (!isalnum(name[i]) && name[i] != '_' && name[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int isValidPassword(const char *password) {
    size_t len = strlen(password);
    if (len < 3 || len > 49) {
        return 0;
    }
    for (int i = 0; password[i]; i++) {
        if (!isalnum(password[i]) && password[i] != '_' && password[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int isValidCountry(const char *country) {
    size_t len = strlen(country);
    if (len < 2 || len > 50) {
        return 0;
    }
    for (int i = 0; country[i]; i++) {
        if (isspace(country[i])) {
            return 0;
        }
        if (!(isalpha((unsigned char)country[i]) || country[i] == '-' || country[i] == '\'' || country[i] == '.')) {
            return 0;
        }
    }
    return 1;
}

void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

        void removeWhitespace(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}
