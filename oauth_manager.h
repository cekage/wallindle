#ifndef OAUTH_MANAGER_H
#define OAUTH_MANAGER_H
//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/param.h>

enum WBoAuth2Field {WBO_ACCESS, REFRESH, SCOPE, TYPE};

typedef struct WBoAuth2Struct {
    char* access_token;

    char* refresh_token;

    char* scope;

    char* token_type;

    // TODO(k) why 32 ?
    uint32_t expires_in;

} WBoAuth2Struct;

static int WBoAuth2Init(WBoAuth2Struct* wbo);

static void  WBoAuth2Cleanup(WBoAuth2Struct* wbo);

static void  WBoAuth2Print(const WBoAuth2Struct* wbo);

static int  WBoAuth2StringSet(const char* content,
                              size_t contentsize, char** wbofield);


#include "./oauth_manager.c"

#endif//OAUTH_MANAGER_H
