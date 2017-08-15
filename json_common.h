#ifndef JSON_COMMON_PARSE_H_INCLUDED
#define JSON_COMMON_PARSE_H_INCLUDED


#include "shared.h"
#include "./lib/jsmn/jsmn.h"

int JsonEquivTo(const char* json, const jsmntok_t*
                tok, const char* s);

char* _WBReadFile(const char* filename, bool isConfig);

char* WBReadConfigFile(const char* filename);

char* WBReadEntriesJsonFile(const char* filename);

char* WBReadoAuthJsonFile(const char* filename);

#endif//JSON_COMMON_PARSE_H_INCLUDED

