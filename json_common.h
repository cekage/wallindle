#ifndef JSON_COMMON_PARSE_H_INCLUDED
#define JSON_COMMON_PARSE_H_INCLUDED

static int JsonEquivTo(const char* json, const jsmntok_t*
                       tok, const char* s);

static char* WBReadJsonFile(const char*
                            filename);
#include "json_common.c"

#endif//JSON_COMMON_PARSE_H_INCLUDED

