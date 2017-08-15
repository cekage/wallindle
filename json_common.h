#ifndef JSON_COMMON_PARSE_H_INCLUDED
#define JSON_COMMON_PARSE_H_INCLUDED

#include "./lib/jsmn/jsmn.h"
#include "shared.h"

wd_result _JsonEquivTo(const char* json, const jsmntok_t*
                       tok, const char* s);

char* WBReadConfigFile(const char* filename);

char* WBReadEntriesJsonFile(const char* filename);

char* WBReadoAuthJsonFile(const char* filename);

#endif//JSON_COMMON_PARSE_H_INCLUDED

