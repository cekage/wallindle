/*
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

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

wd_result WBConfigGet(WBoAuthCred* wbcred);

off_t CheckConfSize(const char* filename, bool check_min_max);


int WBConfigStringSet(const char* content, size_t contentsize,
                      char** wbcfield);

wd_result WBConfigInit(WBoAuthCred* wbc);
void WBConfigCleanup(WBoAuthCred* wbc);

char* WBConfigForgeoAuthURL(WBoAuthCred* wbc);

#endif//CONFIGMANAGER_H
