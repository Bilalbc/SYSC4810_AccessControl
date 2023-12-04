#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <regex.h>

#include "Enrollment.h"
#include "PasswordHashing.h"

static bool verifyPassword(char *password, char *userName);
static bool verifyRole(char *role);
static char* toLowerCase(char *str);
static bool checkCommonPasswords(char *password);
static bool checkNumberFormat(char *password);

const char *commonPasswords[20] = { 
    "123456", "admin", "12345678", "123456789", 
    "1234", "12345", "password", "123",
    "Aa123456", "1234567890", "UNKNOWN", "1234567",
    "123123", "111111", "Password", "12345678910",
    "000000", "admin123", "********", "user"
};

const char *validRoles[9] = {
    "Client", "Premium Client", "Employee", "Financial Planner",
    "Financial Advisor", "Investment Analyst", "Technical Support",
    "Teller", "Compliance Officer"
};

const int NUM_COMMON_PASSWORDS = 20;
const int NUM_ROLES = 9;
const int MAX_INPUT_LENGTH = 25;

void enrollUser() {
    
    char userName[MAX_INPUT_LENGTH];
    char password[MAX_INPUT_LENGTH];
    char userRole[MAX_INPUT_LENGTH];

    bool valid;
    // Get username
    printf("Please enter your user name\n");

    do {
        valid = true;

        fgets(userName, sizeof(userName), stdin);

        userName[strcspn(userName, "\n")] = 0; // remove newline character from input

        if(strlen(userName) > 20) {
            printf("Your Username is to long, Please enter a name up to a maxiumum 20 characters\n");
            valid = false;
            clearBuffer();
            continue;
        } else if(strlen(userName) < 1) {
            printf("You must enter a username up to a maxiumum 20 characters\n");
            valid = false;
            clearBuffer();
            continue;
        }

        if(checkUserExists(userName)) {
            printf("The username %s has already been registered. Please select another one.\n", userName);
            valid = false;
            continue;
        }

    } while(!valid);

    printf("\nYour Username will be: %s \n", userName);


    // Get password
    printf("Please enter your password\n");
    printf("A password must: \n\tBe 8-12 characters in length\n"
        "\tHave one upper-case letter\n"
        "\tHave one lower-case letter\n"
        "\tHave one numerical digit\n"
        "\tand contain atleast one of the following {!, @, #, $, %%, ?, *}\n\n");

    bool passwordValid = false;

    do {
        fgets(password, sizeof(password), stdin);

        password[strcspn(password, "\n")] = 0; // remove newline character from input

        passwordValid = verifyPassword(password, userName);

        if(!passwordValid) clearBuffer();

    } while(!passwordValid);

    printf("\nYour password has been set to %s\n", password);


    // get role
    bool roleValid = false;

    do {
        printf("Please enter your role (type 'help' (case sensitive) for a list of roles)\n");
        
        fgets(userRole, sizeof(userRole), stdin);

        userRole[strcspn(userRole, "\n")] = 0; // remove newline character from input

        roleValid = verifyRole(userRole);

        if(!roleValid) clearBuffer();

    } while(!roleValid);

    printf("\nYour role has been set to %s\n", userRole);


    printf("Saving your user credentials\n");

    saveNewUser(userName, password, userRole);
}

static bool verifyPassword(char *password, char *userName) {
    // Check password length
    if(strlen(password) > 12 || strlen(password) < 8) {
        printf("Your password must be between 8-12 charcters long\n");
        return false;
    }

    // Check matching username 
    if(strcmp(toLowerCase(password), toLowerCase(userName)) == 0) {
        printf("Your password cannot match your username\n");
        return false;
    }

    // Check amount of uppercase, lowercase, numerical and special characters
    int num_upper = 0, num_lower = 0, num_numerical = 0, num_special = 0;
    const char *special_characters = "!@#$%%?*";

    while(*password) {

        if(isupper(*password)) num_upper++;
        if(islower(*password)) num_lower++;
        if(isdigit(*password)) num_numerical++;
        if(strchr(special_characters, *password)) num_special++;
        
        password++;
    }

    if(num_upper == 0) {
        printf("You must have atleast one upper-case letter in your password\n");
        return false;
    }
    else if(num_lower == 0) {
        printf("You must have atleast one lower-case letter in your password\n"); 
        return false;  
    } 
    else if(num_numerical == 0) {
        printf("You must have atleast one numerical value in your password\n");
        return false;
    }
    else if(num_special == 0) {
        printf("You must have atleast one special character in your password\n");
        return false;
    }

    // Check if the password is a common password
    if(!checkCommonPasswords(password)) return false;
    
    // Check if the password matches any common formats
    if(!checkNumberFormat(password)) {
        printf("Your password must not match a date format (MMDDYYYY), license plate format (XXXNNNN), or a phone number format (NNNNNNNNNN)\n");
        return false;
    }

    return true;
}

static bool checkCommonPasswords(char *password) {
    for(int i = 0; i < NUM_COMMON_PASSWORDS; i++) {
        if(strcmp(password, commonPasswords[i]) == 0) {
            return false;
        }
    }
    return true;
}

static bool checkNumberFormat(char *password) {
    const char *datePattern = "^(0[1-9]|1[0-2])(0[1-9]|[1-2][0-9]|3[0-1])(19|20)[0-9]{2}$"; // Example: 12312023
    const char *licensePlatePattern = "^[A-Z0-9]{1,7}$"; // Example: ABC1234
    const char *phoneNumberPattern = "^[0-9]{10}$"; // Example: 1234567890

    regex_t reegex;

    if(regcomp(&reegex, datePattern, REG_EXTENDED) == 0) {
        if(regexec(&reegex, password, 0, NULL, 0) == 0) {
            return false;
        }
    }
    
    if(regcomp(&reegex, licensePlatePattern, REG_EXTENDED) == 0) {
        if(regexec(&reegex, password, 0, NULL, 0) == 0) {
            return false;
        }
    }
    
    if(regcomp(&reegex, phoneNumberPattern, REG_EXTENDED) == 0) {
        if(regexec(&reegex, password, 0, NULL, 0) == 0) {
            return false;
        }
    }
    return true;
}

static bool verifyRole(char *role) {

    bool valid = false;

    if(strcmp(role, "help") == 0) {
        printf("Possible roles: \n"
        "\tClient, Premium Client, Employee, Financial Planner\n"
        "\tFinancial Advisor, Investment Analyst, Technical Support\n"
        "\tTeller, Compliance Officer\n");

        return valid;
    }

    for(int i =0; i < NUM_ROLES; i++) {
        if(strcmp(role, validRoles[i]) == 0) valid = true;
    }

    if(valid == false) printf("Invalid role selction.\n\n");

    return valid;
}

static char* toLowerCase(char *str) {
    char *lowercaseString = malloc(strlen(str));

    for(int i = 0; i < strlen(str); i ++) {
        lowercaseString[i] = tolower(str[i]);
    }
    
    return lowercaseString;
}


void clearBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF && c != '\0');
}