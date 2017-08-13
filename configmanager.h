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

int _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename);

int _WBReadConfiguration(const char* filename,
                                off_t filesize, char** filecontent);

off_t CheckConfSize(const char* filename);

void  WBConfigPrint(const WBoAuthCred* wbc);
int WBConfigStringSet(const char* content,   size_t contentsize,
                             char** wbcfield);

int WBConfigInit(WBoAuthCred* wbc);
void  WBConfigCleanup(WBoAuthCred* wbc);

char* WBConfigForgeoAuthURL(WBoAuthCred* wbc);

bool  WBConfigCompare(WBoAuthCred* wb1, WBoAuthCred* wb2);

#endif//CONFIGMANAGER_H
