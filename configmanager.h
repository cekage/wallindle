#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <sys/types.h>
#include "shared.h"

typedef struct WBoAuthCred {
    char* wallabag_host;
    char* client_id;
    char* client_secret;
    char* username;
    char* password;
    char* token;
} WBoAuthCred;

int WBConfigGet(WBoAuthCred* wbcred);

off_t CheckConfSize(const char* filename, bool check_min_max);


int WBConfigStringSet(const char* content,   size_t contentsize,
                      char** wbcfield);

int WBConfigInit(WBoAuthCred* wbc);
void WBConfigCleanup(WBoAuthCred* wbc);

char* WBConfigForgeoAuthURL(WBoAuthCred* wbc);


#endif//CONFIGMANAGER_H
