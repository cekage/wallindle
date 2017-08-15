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
