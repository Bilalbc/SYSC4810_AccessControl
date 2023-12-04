#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "Enrollment.h"
#include "Login.h"

/*
 * Main class used to execute the prototype
 */

const int MAX_RESPONSE_LENGTH = 10; 

/*
 * Main method to begin the prototype. 
 * prompts the user to enroll a new user or login with an existing user
 */
void main(int argc, char *argv[]) {

    printf("\n\nFinvest Holdings\n");
    printf("Client Holdings and Information System\n");
    printf("-------------------------------------------\n");

    char response[MAX_RESPONSE_LENGTH];
    bool valid = true;

    printf("Are you a new user (Enroll)? Please answer Y/N\n");

    do {
        fgets(response, sizeof(response), stdin);

        response[strcspn(response, "\n")] = 0; // remove newline character from input
        response[0] = toupper(response[0]);

        // Bug fix for stdin not clearing if userinput is greater than buffer size (MAX_INPUT_LENGTH)
        if(strlen(response) == MAX_RESPONSE_LENGTH-1) {
            clearBuffer();
        }

        if(strcmp(response, "Y") == 0) {
            valid = true;
            enrollUser();

        } else if (strcmp(response, "N") == 0) {
            valid = true;
            loginUser();
        
        } else {
            valid = false;
            printf("Please enter either 'Y' or 'N'. \n");
        }

    } while(!valid);

}