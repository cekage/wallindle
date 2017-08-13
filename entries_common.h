#ifndef ENTRIES_COMMON_H_INCLUDED
#define ENTRIES_COMMON_H_INCLUDED

typedef struct WBEntry {
    bool  is_archived;
    bool  is_starred;
    unsigned long int id;
    char* created_at;
} WBEntry;

static char* WBEntryFetchingURL(WBoAuthCred* wbc);

#include "entries_common.c"

#endif//ENTRIES_COMMON_H_INCLUDED
