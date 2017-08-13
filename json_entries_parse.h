#ifndef JSON_PARSE_H_INCLUDED
#define JSON_PARSE_H_INCLUDED

#include "configmanager.h"

static int JsonEntriesParse(const char* jsonresponse,
                            WBoAuthCred* wbc);

#include "json_entries_parse.c"

#endif//JSON_PARSE_H_INCLUDED

