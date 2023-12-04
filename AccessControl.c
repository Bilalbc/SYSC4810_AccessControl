#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AccessControl.h"

const char *matrix[9][9] = { 
	{"RW", "R", "RW", "R", "-", "-", "-", "-", "-" } ,
 	{"RW", "RW", "RW", "R", "R", "-", "-", "-", "-" } ,
	{"R", "R", "-", "-", "-", "-", "-", "-", "-" } ,
	{"RW", "RW", "-", "-", "-", "R", "R", "-", "R" } ,
	{"RW", "RW", "-", "-", "-", "-", "R", "-", "R" } ,
	{"RW", "RW", "-", "-", "-", "R", "R", "R", "R" } ,	
	{"R", "R", "R", "-", "-", "-", "-", "-", "-" } , 
	{"R", "R", "-", "-", "-", "-", "-", "-", "-" } ,
	{"R", "R", "-", "-", "-", "-", "-", "-", "W" } ,
};

const char** getAccountPrivilege(char* accountType) {
	
    accountType[strcspn(accountType, "\n")] = 0; // remove newline character from input
	
	if(strcmp(accountType, "Client") == 0) {
		return matrix[0];
	}
       	
	else if(strcmp(accountType, "Premium Client") == 0) {
		return matrix[1];
	}

	else if(strcmp(accountType, "Employee") == 0) {
		return matrix[2];
	}

	else if(strcmp(accountType, "Financial Planner") == 0) {
		return matrix[3];
	}
	
	else if(strcmp(accountType, "Financial Advisor") == 0) {
		return matrix[4];
	}

	else if(strcmp(accountType, "Investment Analyst") == 0) {
		return matrix[5];
	}

	else if(strcmp(accountType, "Technical Support") == 0) {
		return matrix[6];
	}

	else if(strcmp(accountType, "Teller") == 0) {
		return matrix[7];
	}

	else if(strcmp(accountType, "Compliance Officer") == 0) {
		return matrix[8];
	}

	else {
		return NULL;
	}

}
