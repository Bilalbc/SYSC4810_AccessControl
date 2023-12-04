#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/evp.h>

#include "PasswordHashing.h"

const char *PASSWD_FILE = "passwd.txt";
const char *SEPARATOR = ":";

static void generateSalt(char* salt);
static void generatePasswordHash(char *password, char*  salt, unsigned char *saltedPasswordHash);
static void saveToHashFile(char* userName, char *salt, unsigned char *saltedPasswordHash, char *group_id);
static void getFromHashFile(char* userName, userInfo *user);
static bool comparePasswordWithHash(char *password, userInfo *user);

void saveNewUser(char *userName, char *password, char *role) {
    // SHA256 generates hash of 256 bits, which converted to hex can be represented by 64 hex characters
    // hex = 4 bits, 256 / 4 = 64. a character is 8 bits, so 2 hex values, ie 32 chars to represent 256 bits

	// Set the maximum size Message Digest size for SHA 256, which is 32 characters
    unsigned char saltedPasswordHash[EVP_MD_size(EVP_sha256())];

    // 5 digits + null terminator
    char salt[6];
    generateSalt(salt);

    generatePasswordHash(password, salt, saltedPasswordHash);

    /*
	for (int i = 0; i < 32; i++) 
		printf("%02x", saltedPasswordHash[i]);
	printf("\n");
    */
	saveToHashFile(userName, salt, saltedPasswordHash, role);
}

static void generatePasswordHash(char *password, char *salt, unsigned char *saltedPasswordHash) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    const char *digest_type = "SHA256";
    unsigned char md_value[EVP_MD_size(EVP_sha256())];
    unsigned int md_len, i;

    int saltedPasswordSize = strlen(salt) + strlen(password) + 1;
    char saltedPassword[saltedPasswordSize];

    if (saltedPassword == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    if (snprintf(saltedPassword, saltedPasswordSize, "%s%s", salt, password) >= saltedPasswordSize) {
        printf("Error: Buffer overflow.\n");
        free(saltedPassword);  // Free allocated memory
        exit(1);
    }

    md = EVP_get_digestbyname(digest_type);
    if (md == NULL) {
        printf("Unknown message digest %s\n", password);
        exit(1);
    }

    mdctx = EVP_MD_CTX_new();
    if (!mdctx || !EVP_DigestInit_ex(mdctx, md, NULL)) {
        printf("Message digest initialization failed.\n");
        free(saltedPassword);  // Free allocated memory
        exit(1);
    }

    if (!EVP_DigestUpdate(mdctx, saltedPassword, strlen(saltedPassword))) {
        printf("Message digest update failed.\n");
        free(saltedPassword);  // Free allocated memory
        exit(1);
    }

    if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
        printf("Message digest finalization failed.\n");
        free(saltedPassword);  // Free allocated memory
        exit(1);
    }

    EVP_MD_CTX_free(mdctx);

	snprintf(saltedPasswordHash, md_len, "%s", md_value);
}

static void generateSalt(char *salt) {
    // set seed of the generator to the system clock
    srand(time(0));

    // rand() generates the random number based on the seed, and %99999
    // determines the range of values
    int randomValue = (rand() % 99999);

    sprintf(salt, "%05d", randomValue);
}

userInfo* verifyLogin(char *userName, char *password) {

    userInfo *userinfo = malloc(sizeof(userInfo));

    getFromHashFile(userName, userinfo);

    if(comparePasswordWithHash(password, userinfo)) return userinfo;

    return NULL;
}

static bool comparePasswordWithHash(char *password, userInfo *user) {
    unsigned char saltedPasswordHash[EVP_MD_size(EVP_sha256())];
    char hash[EVP_MAX_MD_SIZE + 1]; // +1 for null terminator

    memset(hash, 0, sizeof(hash)); // set all values to 0
    
    // Create the hash with the entered password to be compared
    generatePasswordHash(password, user->salt, saltedPasswordHash);

    // Convert the generated hash to hex to be compared 
    // with the value in the password file 
    for(int i = 0;i < EVP_MD_size(EVP_sha256()); i++) {
        sprintf(hash + strlen(hash), "%02x", saltedPasswordHash[i]);
    }

    // if they match
    if(strcmp(hash, user->hash) == 0)  return true;

    return false;
}

static void saveToHashFile(char* userName, char *salt, unsigned char *saltedPasswordHash, char *role) {
	FILE *fptr = fopen(PASSWD_FILE, "a");

	if(fptr == NULL) {
		printf("Error printing file");
		return;
	}

	fprintf(fptr, "%s:%s:", userName, salt);

	for(int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {
        // append hex representation of the character 
        // we will have a 64 hex string saved
		fprintf(fptr, "%02x", saltedPasswordHash[i]);
	}
	fprintf(fptr, ":%s\n", role);
	fclose(fptr);
}

bool checkUserExists(char *userName) {
    FILE *fptr = fopen(PASSWD_FILE, "r");
    char *token;

    if(fptr == NULL) {
        printf("Error printing file");
        return false;
    }

    char line[100];
    while(fgets(line, sizeof(line), fptr) != NULL) {
        token = strtok(line, SEPARATOR);
        if(strcmp(userName, token) == 0) {
            return true;
        }
    }
    return false;
}

static void getFromHashFile(char* userName, userInfo *user) {
	FILE *fptr = fopen(PASSWD_FILE, "r");
    char *token;

    if(fptr == NULL) {
        printf("Error printing file");
        return;
    }

    char line[100];
    while(fgets(line, sizeof(line), fptr) != NULL) {
        token = strtok(line, SEPARATOR);

        if(strcmp(userName, token) == 0) {
            strcpy(user->userName, token); // get the userName

            token = strtok(NULL, SEPARATOR); // get the salt
            strcpy(user->salt, token);

            token = strtok(NULL, SEPARATOR); // get the Hash
            strcpy(user->hash, token);

            token = strtok(NULL, SEPARATOR); // get the group type  
            strcpy(user->gid, token);

            fclose(fptr);	
            return;
        }
    }

	fclose(fptr);	
}
