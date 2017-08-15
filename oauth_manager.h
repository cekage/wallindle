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

#ifndef OAUTH_MANAGER_H
#define OAUTH_MANAGER_H

#include <stdint.h>

enum WBoAuth2Field {WBO_ACCESS, REFRESH, SCOPE, TYPE};

typedef struct WBoAuth2Struct {
    char* access_token;

    char* refresh_token;

    char* scope;

    char* token_type;

    // TODO(k) why 32 ?
    uint32_t expires_in;

} WBoAuth2Struct;

wd_result WBoAuth2Init(WBoAuth2Struct* wbo);
void WBoAuth2Cleanup(WBoAuth2Struct* wbo);
void WBoAuth2Print(const WBoAuth2Struct* wbo);

int WBoAuth2StringSet(const char* content,
                      size_t contentsize, char** wbofield);


#endif//OAUTH_MANAGER_H
