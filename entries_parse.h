#ifndef ENTRIES_PARSE_H_INCLUDED
#define ENTRIES_PARSE_H_INCLUDED

static void  WBReadJsonEntries(WBoAuthCred* wbc, const char* filename);

static void  WBReadDownloadedJsonEntries(const char* jsoncontent,
        WBoAuthCred* wbc);


#include "entries_parse.c"

#endif//ENTRIES_PARSE_H_INCLUDED
