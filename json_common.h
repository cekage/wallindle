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

#ifndef JSON_COMMON_PARSE_H_INCLUDED
#define JSON_COMMON_PARSE_H_INCLUDED

#include "./lib/jsmn/jsmn.h"
#include "shared.h"

// TODO(k) Add test
wd_result _JsonEquivTo(const char* json, const jsmntok_t*
                       tok, const char* s);

unsigned int _GetTokenCount(const char* jsonresponse);
unsigned int _GetTokenCountExt(const char* jsonresponse, jsmntok_t* tokens,
                               unsigned int maxtoken);

#endif//JSON_COMMON_PARSE_H_INCLUDED

