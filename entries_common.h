#ifndef ENTRIES_COMMON_H_INCLUDED
#define ENTRIES_COMMON_H_INCLUDED

#include "configmanager.h"

typedef struct WBEntry {
    bool  is_archived;
    bool  is_starred;
    unsigned long int id;
    char* created_at;
} WBEntry;

char* WBEntryFetchingURL(WBoAuthCred* wbc);

void _PrintEntry(const WBEntry* wbe);

#endif//ENTRIES_COMMON_H_INCLUDED
