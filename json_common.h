#ifndef JSON_COMMON_PARSE_H_INCLUDED
#define JSON_COMMON_PARSE_H_INCLUDED


#include "./lib/jsmn/jsmn.h"
//#include "./lib/jsmn/jsmn.c"

int JsonEquivTo(const char* json, const jsmntok_t*
                       tok, const char* s);

char* WBReadJsonFile(const char*
                            filename);

#endif//JSON_COMMON_PARSE_H_INCLUDED

