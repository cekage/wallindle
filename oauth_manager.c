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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <inttypes.h>

#include "shared.h"

#include "oauth_manager.h"


// TODO(k)Assigndefaultnullvalueshttps://stackoverflow.com/a/749690
wd_result WBoAuth2Init(WBoAuth2Struct* wbo) {
#define CHECKFIELD(FD) result &= (NULL != wbo->FD);
#define INITFIELD(FD) wbo->FD = calloc(1, sizeof(char));CHECKFIELD(FD)
    bool result = true;
    INITFIELD(access_token);
    INITFIELD(refresh_token);
    INITFIELD(scope);
    INITFIELD(token_type);
    wbo->expires_in = 0;
    return result ? WNDL_OK : WNDL_ERROR;
#undef INITFIELD
#undef CHECKFIELD
}

void WBoAuth2Cleanup(WBoAuth2Struct* wbo) {
#define FREEFIELD(FD) free(wbo->FD);wbo->FD = NULL;
    FREEFIELD(access_token);
    FREEFIELD(refresh_token);
    FREEFIELD(scope);
    FREEFIELD(token_type);
    wbo->expires_in = 0;
#undef FREEFIELD
}

void WBoAuth2Print(const WBoAuth2Struct* wbo) {
#define PRINTFIELD(FD) printf("wbo->%s(%" PRIuPTR "o.)=%s\n", #FD, strlen(wbo->FD), wbo->FD)
    printf("\n");
    PRINTFIELD(access_token);
    PRINTFIELD(refresh_token);
    PRINTFIELD(scope);
    PRINTFIELD(token_type);
    printf("wbo->token_type=%u\n", wbo->expires_in);
#undef PRINTFIELD
}

int WBoAuth2StringSet(const char* content, size_t contentsize,
                      char** wbofield) {
    return StoreContent(content, contentsize, wbofield);
}
