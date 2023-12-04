#ifndef PASSWORDHASHING_h
#define PASSWORDHASHING_h

#include <openssl/evp.h>
#include <stdbool.h>

typedef struct userInfo {
    char userName[32];
    char salt[32];
    char hash[EVP_MAX_MD_SIZE + 1]; // +1 for null terminator
    char gid[32]; // group identifier number
} userInfo;

void saveNewUser(char *userName, char *password, char *role);
bool checkUserExists(char *userName);
userInfo* verifyLogin(char *userName, char *password);

#endif