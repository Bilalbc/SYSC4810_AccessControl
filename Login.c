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

int loginUser() {

    char userName[25];
    char password[25];
    char userRole[50];

    bool foundUser = false;
    bool passwordCorrect = false;

    userInfo *user;

    printf("User Name: \t");

    do {
        fgets(userName, sizeof(userName), stdin);

        userName[strcspn(userName, "\n")] = 0; // remove newline character from input

        if(checkUserExists(userName)) foundUser = true; 
        else {
            printf("User %s could not be found\n", userName);
            clearBuffer();
        }

    } while(!foundUser);


    printf("Password:\t");

    do {
        fgets(password, sizeof(password), stdin);

        password[strcspn(password, "\n")] = 0; // remove newline character from input

        user = verifyLogin(userName, password);

        if(user != NULL) passwordCorrect = true;
        else {
            printf("Password was incorrect, please try again\n");
            clearBuffer();
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