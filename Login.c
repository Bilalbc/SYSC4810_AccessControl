#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Enrollment.h"
#include "PasswordHashing.h"
#include "AccessControl.h"
#include "Login.h"

const char *subjects[9] = {"Client", "Premium Client", "Employee", 
                            "Financial Planner", "Financial Advisor", "Investment Analyst", 
                            "Technical Support", "Teller", "Compliance Officer"
                            };


const char *objects[10] = {"Client Account Balance", "Client Investments Portfolio", "Client Information", 
                            "Financial Advisor Details", "Investment Analyst Details", "Money Market Instruments",
                             "Private consumer instruments", "Derivatives trading", "Interest instruments", 
                             "System Access (Outside of 0900 - 1700)"
                            };

const int NUM_OBJECTS = 10;
const int MAX_LOGIN_INPUT_LENGTH = 50;

int loginUser() {

    char userName[MAX_LOGIN_INPUT_LENGTH];
    char password[MAX_LOGIN_INPUT_LENGTH];
    char userRole[MAX_LOGIN_INPUT_LENGTH];

    bool foundUser = false;
    bool passwordCorrect = false;

    userInfo *user;
    
    do {
        printf("User Name:\t");
        fgets(userName, sizeof(userName), stdin);

        userName[strcspn(userName, "\n")] = 0; // remove newline character from input

        // Bug fix for stdin not clearing if userinput is greater than buffer size (MAX_INPUT_LENGTH)
        if(strlen(userName) == MAX_LOGIN_INPUT_LENGTH-1) {
            clearBuffer();
        }

        if(checkUserExists(userName)) foundUser = true; 
        else {
            printf("User %s could not be found\n", userName);
        }

    } while(!foundUser);

    do {
        printf("Password:\t");
        fgets(password, sizeof(password), stdin);

        password[strcspn(password, "\n")] = 0; // remove newline character from input

        // Bug fix for stdin not clearing if userinput is greater than buffer size (MAX_INPUT_LENGTH)
        if(strlen(password) == MAX_LOGIN_INPUT_LENGTH-1) {
            clearBuffer();
        }

        user = verifyLogin(userName, password);

        if(user != NULL) passwordCorrect = true;
        else {
            printf("Password was incorrect, please try again\n");
        }

    } while(!passwordCorrect);

    printf("\n\nLogin successful. Logged in as user %s\n", userName);
    printf("Your role is: %s\n", user->gid);

    
    printf("Your have the following access privileges (object : privilege): \n\n\n");

    const char **privileges = getAccountPrivilege(user->gid);

    if (privileges != NULL) {
        for(int i = 0; i < NUM_OBJECTS; i++) {
            printf("%s : %s\n", objects[i], privileges[i]);
        }
    } else {
        printf("Unknown account type: %s\n", user->gid);
    }
}