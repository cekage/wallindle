#ifndef JSON_OAUTH_PARSE_H_INCLUDED
#define JSON_OAUTH_PARSE_H_INCLUDED

#include "./lib/jsmn/jsmn.h"
#include "./lib/jsmn/jsmn.c"

#include "configmanager.h"


static char* ExtractToken(const char* jsonresponse);

#include "json_oauth_parse.c"

#endif//JSON_OAUTH_PARSE_H_INCLUDED

