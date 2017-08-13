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

static int WBConfigGet(WBoAuthCred* wbcred);

static int _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename);

static int _WBReadConfiguration(const char* filename,
                                off_t filesize, char** filecontent);

static off_t CheckConfSize(const char* filename);

static void  WBConfigPrint(const WBoAuthCred* wbc);
static int WBConfigStringSet(const char* content,   size_t contentsize,
                             char** wbcfield);

static int WBConfigInit(WBoAuthCred* wbc);
static void  WBConfigCleanup(WBoAuthCred* wbc);

static char* WBConfigForgeoAuthURL(WBoAuthCred* wbc);

static bool  WBConfigCompare(WBoAuthCred* wb1, WBoAuthCred* wb2);

#include "configmanager.c"

#endif//CONFIGMANAGER_H
