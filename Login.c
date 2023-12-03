#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "passwordHashing.h"
#include "accessControl.h"
#include "Login.h"

int loginUser() {

    char userName[25];
    char password[25];
    char userRole[25];

    bool foundUser = false;
    bool passwordCorrect = false;

    userInfo *user;

    printf("Please enter your user name\n");

    do {
        fgets(userName, sizeof(userName), stdin);

        userName[strcspn(userName, "\n")] = 0; // remove newline character from input

        if(checkUserExists(userName)) foundUser = true; 
        else printf("User %s could not be found\n", userName);

    } while(!foundUser);


    printf("Please enter the password for %s\n", userName);

    do {
        fgets(password, sizeof(password), stdin);

        password[strcspn(password, "\n")] = 0; // remove newline character from input

        user = verifyLogin(userName, password);

        if(user != NULL) passwordCorrect = true;
        else printf("Password was incorrect, please try again\n");

    } while(!passwordCorrect);

    printf("\n\n\nLogin successful. Logged in as user %s\n", userName);
    printf("Your is role: %s", user->gid);

    
    printf("Your have the following access privileges (object : privilege): \n\n");

    const char **privileges = getAccountPrivilege(user->gid);

    if (privileges != NULL) {
        for(int i = 0; i < NUM_OBJECTS; i++) {
            printf("%s : %s\n", objects[i], privileges[i]);
        }
    } else {
        printf("Unknown account type: %s\n", user->gid);
    }
}