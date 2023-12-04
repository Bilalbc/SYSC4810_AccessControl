#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <regex.h>

#include "Enrollment.h"
#include "PasswordHashing.h"

static bool verifyUsername(char *userName);
static bool verifyPassword(char *password, char *userName);
static bool verifyRole(char *role);
static char* toLowerCase(char *str);
static bool checkCommonPasswords(char *password);
static bool checkNumberFormat(char *password);

const char *commonPasswords[14] = { 
    "P@ssw0rd", "Pass@123", "Admin@123", "Demo@123", 
    "Aa123456@", "Abcd@1234", "Password@123", "Abcd@123",
    "Welcome@123", "Pass@1234", "India@123", "Kumar@123",
    "Test@123", "Abc@1234"
};

const char *validRoles[9] = {
    "Client", "Premium Client", "Employee", "Financial Planner",
    "Financial Advisor", "Investment Analyst", "Technical Support",
    "Teller", "Compliance Officer"
};

const int NUM_COMMON_PASSWORDS = 14;
const int NUM_ROLES = 9;
const int MAX_ENROLL_INPUT_LENGTH = 25;

void enrollUser() {
    
    char userName[MAX_ENROLL_INPUT_LENGTH];
    char password[MAX_ENROLL_INPUT_LENGTH];
    char userRole[MAX_ENROLL_INPUT_LENGTH];

    bool userNameValid = false;

    // Get username
    do {
        printf("\nUsername:\t");

        fgets(userName, sizeof(userName), stdin);

        userName[strcspn(userName, "\n")] = 0; // remove newline character from input

        // Bug fix for stdin not clearing if userinput is greater than buffer size (MAX_INPUT_LENGTH)
        if(strlen(userName) == MAX_ENROLL_INPUT_LENGTH-1) {
            clearBuffer();
        }

        userNameValid = verifyUsername(userName);

    } while(!userNameValid);

    printf("Username will be %s.\n", userName);

    // Get password
    printf("\nPlease enter a password\n");
    printf("A password must: \n\tBe 8-12 characters in length\n"
        "\tHave one upper-case letter\n"
        "\tHave one lower-case letter\n"
        "\tHave one numerical digit\n"
        "\tand contain atleast one of the following {!, @, #, $, %%, ?, *}\n\n");

    bool passwordValid = false;

    do {
        printf("\nPassword:\t");
        fgets(password, sizeof(password), stdin);

        password[strcspn(password, "\n")] = 0; // remove newline character from input

        // Bug fix for stdin not clearing if userinput is greater than buffer size (MAX_INPUT_LENGTH)
        if(strlen(password) == MAX_ENROLL_INPUT_LENGTH-1) {
            clearBuffer();
        }

        passwordValid = verifyPassword(password, userName);

    } while(!passwordValid);

    // get role
    bool roleValid = false;

    printf("\nPlease enter your role (case sensitive) (type 'help' for a list of roles)\n");

    do {
        printf("\nRole:\t\t");
        fgets(userRole, sizeof(userRole), stdin);

        userRole[strcspn(userRole, "\n")] = 0; // remove newline character from input

        // Bug fix for stdin not clearing if userinput is greater than buffer size (MAX_INPUT_LENGTH)
        if(strlen(userRole) == MAX_ENROLL_INPUT_LENGTH-1) {
            clearBuffer();
        }

        roleValid = verifyRole(userRole);

    } while(!roleValid);

    printf("\nCreating new user...\n");

    saveNewUser(userName, password, userRole);
}

static bool verifyUsername(char *userName) {
    if(strlen(userName) > 20) {
        printf("Your Username is to long, Please enter a name up to a maxiumum of 20 characters\n");
        return false;

    } else if(strlen(userName) < 1) {
        printf("You must enter a username up to a maxiumum of 20 characters\n");
        return false;
    }

    if(checkUserExists(userName)) {
        printf("The username %s has already been registered. Please select another one.\n", userName);
        return false;
    }
    return true;
}

static bool verifyPassword(char *password, char *userName) {
    // Check password length
    if(strlen(password) > 12 || strlen(password) < 8) {
        printf("Your password must be between 8-12 charcters long\n");
        return false;
    }

    // Check amount of uppercase, lowercase, numerical and special characters
    int num_upper = 0, num_lower = 0, num_numerical = 0, num_special = 0;
    const char *special_characters = "!@#$%%?*";
    
    for(int i = 0; i < strlen(password); i++) {
        if(isupper(password[i])) num_upper++;
        if(islower(password[i])) num_lower++;
        if(isdigit(password[i])) num_numerical++;
        if(strchr(special_characters, password[i])) num_special++;
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

    // Check matching username 
    if(strcmp(toLowerCase(password), toLowerCase(userName)) == 0) {
        printf("Your password cannot match your username\n");
        return false;
    }

    // Check if the password is a common password
    if(!checkCommonPasswords(password)) {
        printf("Your password is too common, please select another.\n");
        return false;
    }

    // Check if the password matches any common formats
    if(!checkNumberFormat(password)) {
        printf("Your password must not match a date format (MMDDYYYY), "
                "license plate format (XXXNNNN), or a phone number format (NNNNNNNNNN)\n");
        return false;
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

    if(valid == false) printf("Invalid role selction.\n");

    return valid;
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