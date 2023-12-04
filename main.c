#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "Enrollment.h"
#include "Login.h"

void main(int argc, char *argv[]) {

    printf("Finvest Holdings\n");
    printf("Client Holdings and Information System\n");
    printf("-------------------------------------------\n");

    char response[10];
    bool valid = true;

    printf("Are you a new user (Enroll)? Please answer Y/N\n");

    do {
        fgets(response, sizeof(response), stdin);

        response[strcspn(response, "\n")] = 0; // remove newline character from input
        response[0] = toupper(response[0]);
        
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